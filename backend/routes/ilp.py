from flask import Blueprint, jsonify
from pathlib import Path
import subprocess
import re


ilp_bp = Blueprint("ilp", __name__)


@ilp_bp.route("/api/v1/algorithms/ilp", methods=["GET"])
def run_ilp():

    executable = (
        Path(__file__).resolve().parent.parent.parent
        / "algorithms"
        / "unit6"
        / "ilp"
        / "ilp.exe"
    )

    if not executable.is_file():
        return jsonify({
            "success": False,
            "message": "ILP executable not found.",
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
                "message": "ILP C++ execution failed.",
                "error": process.stderr,
                "output": output
            }), 500

        # Extract decision variables and their locations.
        variable_matches = re.findall(
            r"^\s*x(\d+)\s*=\s*1\s*if\s*(.+?)\s*is selected\s*$",
            output,
            re.M | re.I
        )

        locations = {
            int(number): location.strip()
            for number, location in variable_matches
        }

        if not locations:
            return jsonify({
                "success": False,
                "message": "Could not extract ILP decision variables.",
                "output": output
            }), 500

        number_of_variables = len(locations)

        # Extract objective function.
        objective_match = re.search(
            r"Objective Function:\s*(.*?)\n\s*Constraints:",
            output,
            re.S | re.I
        )

        objective_function = (
            " ".join(objective_match.group(1).split())
            if objective_match
            else "Minimize " + " + ".join(
                f"x{i}" for i in range(1, number_of_variables + 1)
            )
        )

        # Extract linear constraints.
        constraints_match = re.search(
            r"Constraints:\s*(.*?)\n\s*Binary constraints:",
            output,
            re.S | re.I
        )

        constraints_section = (
            constraints_match.group(1)
            if constraints_match
            else ""
        )

        constraint_matches = re.findall(
            r"^\s*C(\d+):\s*x(\d+)\s*\+\s*x(\d+)\s*>=\s*1\s*$",
            constraints_section,
            re.M | re.I
        )

        # Extract optimal binary solution.
        solution_match = re.search(
            r"Optimal ILP Solution:\s*(.*?)"
            r"\n\s*Minimum objective value",
            output,
            re.S | re.I
        )

        solution_section = (
            solution_match.group(1)
            if solution_match
            else ""
        )

        assignment_matches = re.findall(
            r"^\s*x(\d+)\s*=\s*([01])\s*$",
            solution_section,
            re.M | re.I
        )

        assignments = {
            int(number): int(value)
            for number, value in assignment_matches
        }

        if len(assignments) != number_of_variables:
            return jsonify({
                "success": False,
                "message": "Could not extract the complete ILP solution.",
                "output": output
            }), 500

        # Extract minimum objective value.
        objective_match = re.search(
            r"Minimum objective value\s*=\s*(\d+)",
            output,
            re.I
        )

        if not objective_match:
            return jsonify({
                "success": False,
                "message": "Could not extract the objective value.",
                "output": output
            }), 500

        minimum_objective_value = int(objective_match.group(1))

        # Extract selected locations.
        selected_match = re.search(
            r"Selected locations:\s*(.*?)"
            r"\n\s*-{5,}",
            output,
            re.S | re.I
        )

        selected_locations = (
            re.findall(
                r"^\s*-\s*(.+?)\s*$",
                selected_match.group(1),
                re.M
            )
            if selected_match
            else []
        )

        # Build decision-variable details.
        decision_variables = []

        for index in sorted(locations):
            value = assignments[index]

            decision_variables.append({
                "variable": f"x{index}",
                "location": locations[index],
                "value": value,
                "selected": value == 1,
                "meaning": (
                    "Location selected"
                    if value == 1
                    else "Location not selected"
                )
            })

        # Build and independently verify every edge constraint.
        constraints = []
        all_constraints_satisfied = True

        for number, u, v in constraint_matches:
            u = int(u)
            v = int(v)

            left_value = assignments.get(u, 0)
            right_value = assignments.get(v, 0)

            satisfied = left_value + right_value >= 1

            if not satisfied:
                all_constraints_satisfied = False

            constraints.append({
                "constraint": f"C{number}",
                "expression": f"x{u} + x{v} >= 1",
                "source": locations.get(u, f"x{u}"),
                "destination": locations.get(v, f"x{v}"),
                "left_value": left_value,
                "right_value": right_value,
                "satisfied": satisfied
            })

        # Independently check the objective value.
        calculated_objective = sum(assignments.values())

        objective_matches = (
            calculated_objective == minimum_objective_value
        )

        solution_verified = (
            all_constraints_satisfied
            and objective_matches
            and len(constraints) > 0
            and set(selected_locations) == {
                locations[i]
                for i, value in assignments.items()
                if value == 1
            }
        )

        # Extract binary restrictions.
        binary_constraints = [
            f"x{i} in {{0,1}}"
            for i in sorted(locations)
        ]

        verification_match = re.search(
            r"Verification:\s*(.*?)\n\s*-{5,}",
            output,
            re.S | re.I
        )

        verification = (
            " ".join(verification_match.group(1).split())
            if verification_match
            else "Verification message unavailable."
        )

        return jsonify({
            "success": True,

            "algorithm": "Integer Linear Programming (0-1 ILP)",

            "method": "Brute Force Enumeration of Binary Assignments",

            "number_of_variables": number_of_variables,

            "number_of_constraints": len(constraints),

            "decision_variables": decision_variables,

            "objective_function": objective_function,

            "objective_type": "MINIMIZATION",

            "constraints": constraints,

            "binary_constraints": binary_constraints,

            "optimal_solution": {
                f"x{i}": assignments[i]
                for i in sorted(assignments)
            },

            "minimum_objective_value": minimum_objective_value,

            "calculated_objective_value": calculated_objective,

            "selected_locations": selected_locations,

            "all_constraints_satisfied": all_constraints_satisfied,

            "objective_matches": objective_matches,

            "solution_verified": solution_verified,

            "result": (
                "OPTIMAL ILP SOLUTION FOUND"
                if solution_verified
                else "ILP SOLUTION VERIFICATION FAILED"
            ),

            "time_complexity": "O(2^V * (V + E))",

            "space_complexity": (
                "O(V) auxiliary space; O(V + E) including graph input"
            ),

            "cpp_output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "ILP execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": "An error occurred while running ILP.",
            "error": str(error)
        }), 500