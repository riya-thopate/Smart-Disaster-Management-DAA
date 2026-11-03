import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

subset_sum_bp = Blueprint("subset_sum", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

SUBSET_SUM_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit4"
    / "backtracking"
    / "subset_sum.exe"
)


@subset_sum_bp.get("/api/v1/algorithms/subset-sum")
def run_subset_sum():

    try:
        if not SUBSET_SUM_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Subset Sum executable not found.",
                "expected_path": str(SUBSET_SUM_EXE)
            }), 500

        result = subprocess.run(
            [str(SUBSET_SUM_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Subset Sum execution failed."
            }), 500

        output = result.stdout

        # Extract available resource quantities.
        resources_match = re.search(
            r"Available emergency resource quantities:\s*\n(.*?)"
            r"Required target quantity",
            output,
            re.DOTALL
        )

        if not resources_match:
            raise ValueError("Resource quantities section not found.")

        resources = [
            int(value)
            for value in re.findall(
                r"\d+",
                resources_match.group(1)
            )
        ]

        # Extract target quantity.
        target_match = re.search(
            r"Required target quantity\s*=\s*(\d+)",
            output
        )

        if not target_match:
            raise ValueError("Target quantity not found.")

        target = int(target_match.group(1))

        # Check whether a subset was found.
        if "Subset found:" in output:

            subset_match = re.search(
                r"Subset found:\s*\n(.*?)"
                r"Subset sum\s*=",
                output,
                re.DOTALL
            )

            if not subset_match:
                raise ValueError("Selected subset section not found.")

            selected_resources = [
                int(value)
                for value in re.findall(
                    r"\d+",
                    subset_match.group(1)
                )
            ]

            subset_sum_match = re.search(
                r"Subset sum\s*=\s*(\d+)",
                output
            )

            if not subset_sum_match:
                raise ValueError("Subset sum result not found.")

            subset_sum = int(subset_sum_match.group(1))

            if sum(selected_resources) != subset_sum:
                raise ValueError("Selected resource sum is inconsistent.")

            if subset_sum != target:
                raise ValueError("Selected subset does not match target.")

            subset_found = True

        elif "No subset can produce the target." in output:
            selected_resources = []
            subset_sum = 0
            subset_found = False

        else:
            raise ValueError("Subset Sum result not found.")

        return jsonify({
            "success": True,
            "algorithm": "Subset Sum",
            "method": "Backtracking",
            "resources": resources,
            "target": target,
            "subset_found": subset_found,
            "selected_resources": selected_resources,
            "subset_sum": subset_sum,
            "time_complexity": "O(2^n)",
            "space_complexity": "O(n)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Subset Sum execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500