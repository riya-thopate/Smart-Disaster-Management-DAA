from flask import Blueprint, jsonify
from pathlib import Path
import subprocess
import re


three_sat_bp = Blueprint("three_sat", __name__)


@three_sat_bp.route("/api/v1/algorithms/three-sat", methods=["GET"])
def run_three_sat():

    executable = (
    Path(__file__).resolve().parent.parent.parent
    / "algorithms"
    / "unit6"
    / "three_sat"
    / "three_sat.exe"
)
    if not executable.is_file():
        return jsonify({
            "success": False,
            "message": "3-SAT executable not found.",
            "expected_path": str(executable)
        }), 404

    try:
        process = subprocess.run(
            [str(executable)],
            capture_output=True,
            text=True,
            timeout=10,
            cwd=str(executable.parent)
        )

        output = process.stdout or ""

        if process.returncode != 0:
            return jsonify({
                "success": False,
                "message": "3-SAT C++ program execution failed.",
                "error": process.stderr,
                "output": output
            }), 500

        # Extract the 3-CNF formula.
        formula_match = re.search(
            r"3-CNF Formula:\s*(.*?)\n\s*-{5,}",
            output,
            re.S | re.I
        )

        formula = (
            formula_match.group(1).strip()
            if formula_match
            else (
                "(A OR B OR C)\n"
                "AND (!A OR B OR !C)\n"
                "AND (A OR !B OR C)"
            )
        )

        # Extract Boolean variable assignments.
        variable_meanings = {
            "A": "Water Available",
            "B": "Food Available",
            "C": "Medicine Available"
        }

        satisfying_assignment = {}

        for variable in variable_meanings:
            match = re.search(
                rf"^\s*{variable}\s*=\s*(TRUE|FALSE)\s*$",
                output,
                re.M | re.I
            )

            if match:
                satisfying_assignment[variable] = (
                    match.group(1).upper() == "TRUE"
                )

        # Extract individual clause results.
        clause_expressions = {
            1: "(A OR B OR C)",
            2: "(!A OR B OR !C)",
            3: "(A OR !B OR C)"
        }

        clauses = []

        clause_matches = re.findall(
            r"^\s*Clause\s+([123])\s*=\s*(TRUE|FALSE)\s*$",
            output,
            re.M | re.I
        )

        for number, value in clause_matches:
            clause_number = int(number)

            clauses.append({
                "number": clause_number,
                "expression": clause_expressions[clause_number],
                "value": value.upper() == "TRUE"
            })

        # Extract the final SAT result.
        result_match = re.search(
            r"Result:\s*(3-CNF FORMULA IS (?:UN)?SATISFIABLE)",
            output,
            re.I
        )

        if not result_match:
            return jsonify({
                "success": False,
                "message": "Could not parse the 3-SAT result.",
                "output": output
            }), 500

        result = result_match.group(1).upper()
        satisfiable = result == "3-CNF FORMULA IS SATISFIABLE"

        # Prepare variable information for the frontend.
        variables = [
            {
                "variable": variable,
                "meaning": meaning
            }
            for variable, meaning in variable_meanings.items()
        ]

        return jsonify({
            "success": True,
            "algorithm": "3-SAT",

            "method": "Brute Force Boolean Assignment Enumeration",

            "number_of_variables": 3,
            "number_of_clauses": 3,

            "formula": formula,

            "variables": variables,

            "satisfiable": satisfiable,

            "satisfying_assignment": (
                satisfying_assignment if satisfiable else {}
            ),

            "clauses": clauses,

            "all_clauses_true": (
                len(clauses) == 3
                and all(clause["value"] for clause in clauses)
            ),

            "result": result,

            "time_complexity": "O(2^n)",
            "space_complexity": "O(1) auxiliary space",

            "cpp_output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "3-SAT execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": "An error occurred while running 3-SAT.",
            "error": str(error)
        }), 500