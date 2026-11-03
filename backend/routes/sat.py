import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

sat_bp = Blueprint("sat", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

SAT_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit6"
    / "sat"
    / "sat.exe"
)


@sat_bp.get("/api/v1/algorithms/sat")
def run_sat():

    try:
        if not SAT_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "SAT executable not found.",
                "expected_path": str(SAT_EXE)
            }), 500

        result = subprocess.run(
            [str(SAT_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "SAT execution failed."
            }), 500

        output = result.stdout

        # Extract Boolean formula.
        formula_match = re.search(
            r"Boolean Formula:\s*\n\s*(.+)",
            output
        )

        if not formula_match:
            raise ValueError("Boolean formula not found.")

        formula = formula_match.group(1).strip()

              # Extract variable descriptions only.
        # Do not include satisfying assignments.
        variables_section = re.search(
            r"Boolean variables:\s*\n(.*?)\n\s*Boolean Formula:",
            output,
            re.DOTALL
        )

        if not variables_section:
            raise ValueError("Boolean variables section not found.")

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

        # Extract SAT or UNSAT result.
        status_match = re.search(
            r"Result:\s*(SATISFIABLE|UNSATISFIABLE)",
            output
        )

        if not status_match:
            raise ValueError("SAT result not found.")

        status = status_match.group(1)
        satisfiable = status == "SATISFIABLE"

        # Extract the first satisfying assignment.
        assignments = {}

        if satisfiable:

            assignment_matches = re.findall(
                r"^\s*([A-Z])\s*=\s*(TRUE|FALSE)\s*$",
                output,
                re.MULTILINE
            )

            assignments = {
                variable: value == "TRUE"
                for variable, value in assignment_matches
            }

            if not assignments:
                raise ValueError(
                    "Satisfying assignment not found."
                )

        return jsonify({
            "success": True,
            "algorithm": "Boolean SAT",
            "method": "Brute Force / Truth Table Enumeration",
            "number_of_variables": len(variables),
            "variables": variables,
            "formula": formula,
            "satisfiable": satisfiable,
            "result": status,
            "satisfying_assignment": assignments,
            "time_complexity": "O(2^n)",
            "space_complexity": "O(1) auxiliary for this implementation",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "SAT execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500