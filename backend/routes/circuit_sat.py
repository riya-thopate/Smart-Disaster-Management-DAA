import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

circuit_sat_bp = Blueprint("circuit_sat", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

CIRCUIT_SAT_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit6"
    / "circuit_sat"
    / "circuit_sat.exe"
)


@circuit_sat_bp.get("/api/v1/algorithms/circuit-sat")
def run_circuit_sat():

    try:
        if not CIRCUIT_SAT_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Circuit-SAT executable not found.",
                "expected_path": str(CIRCUIT_SAT_EXE)
            }), 500

        result = subprocess.run(
            [str(CIRCUIT_SAT_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": (
                    result.stderr or
                    "Circuit-SAT execution failed."
                )
            }), 500

        output = result.stdout

        # Extract input-variable descriptions only.
        variables_section = re.search(
            r"Circuit inputs:\s*\n(.*?)\n\s*Circuit Formula:",
            output,
            re.DOTALL
        )

        if not variables_section:
            raise ValueError("Circuit input section not found.")

        variable_matches = re.findall(
            r"^\s*([A-Z])\s*=\s*(.+?)\s*$",
            variables_section.group(1),
            re.MULTILINE
        )

        variables = [
            {
                "variable": variable,
                "meaning": meaning.strip()
            }
            for variable, meaning in variable_matches
        ]

        # Extract Boolean circuit formula.
        formula_match = re.search(
            r"Circuit Formula:\s*\n\s*(.+)",
            output
        )

        if not formula_match:
            raise ValueError("Circuit formula not found.")

        formula = formula_match.group(1).strip()

        # Extract the satisfying assignment.
              # Extract the satisfying assignment.
        assignment_section = re.search(
            r"Satisfying input found:\s*\n(.*?)"
            r"(?=\n\s*OR Gate Output)",
            output,
            re.DOTALL
        )

        assignments = {}

        if assignment_section:

            assignment_matches = re.findall(
                r"^\s*([A-Z])\s*=\s*(TRUE|FALSE)\s*$",
                assignment_section.group(1),
                re.MULTILINE
            )

            assignments = {
                variable: value == "TRUE"
                for variable, value in assignment_matches
            }

        # Extract gate outputs from the COMPLETE C++ output.
        or_match = re.search(
            r"OR Gate Output\s*=\s*(TRUE|FALSE)",
            output
        )

        final_match = re.search(
            r"Final Circuit Output\s*=\s*(TRUE|FALSE)",
            output
        )

        or_gate_output = (
            or_match.group(1) == "TRUE"
            if or_match else None
        )

        final_output = (
            final_match.group(1) == "TRUE"
            if final_match else None
        )

        # Extract the overall result.
        status_match = re.search(
            r"Result:\s*(CIRCUIT SATISFIABLE|CIRCUIT UNSATISFIABLE)",
            output
        )

        if not status_match:
            raise ValueError("Circuit-SAT result not found.")

        status = status_match.group(1)
        satisfiable = status == "CIRCUIT SATISFIABLE"

        if satisfiable:
            if len(assignments) != len(variables):
                raise ValueError(
                    "Satisfying assignment is incomplete."
                )

            if or_gate_output is not True or final_output is not True:
                raise ValueError(
                    "Extracted gate outputs are inconsistent."
                )
        else:
            assignments = {}

        return jsonify({
            "success": True,
            "algorithm": "Circuit-SAT",
            "method": "Brute Force Boolean Circuit Evaluation",
            "number_of_inputs": len(variables),
            "variables": variables,
            "formula": formula,
            "satisfiable": satisfiable,
            "result": status,
            "satisfying_assignment": assignments,
            "or_gate_output": or_gate_output,
            "final_circuit_output": final_output,
            "time_complexity": "O(2^n)",
            "space_complexity": "O(1) auxiliary space",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Circuit-SAT execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500