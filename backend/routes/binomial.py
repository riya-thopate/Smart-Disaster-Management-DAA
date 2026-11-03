import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

binomial_bp = Blueprint("binomial", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

BINOMIAL_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "binomial"
    / "binomial.exe"
)


@binomial_bp.get("/api/v1/algorithms/binomial")
def run_binomial():

    try:
        if not BINOMIAL_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Binomial executable not found."
            }), 500

        result = subprocess.run(
            [str(BINOMIAL_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Binomial execution failed."
            }), 500

        output = result.stdout

        n_match = re.search(
            r"Available response teams:\s*(\d+)",
            output
        )

        k_match = re.search(
            r"Teams to select:\s*(\d+)",
            output
        )

        result_match = re.search(
            r"C\(\s*(\d+)\s*,\s*(\d+)\s*\)\s*=\s*(\d+)",
            output
        )

        if not all([n_match, k_match, result_match]):
            return jsonify({
                "success": False,
                "message": "Could not extract Binomial Coefficient result.",
                "output": output
            }), 500

        n = int(n_match.group(1))
        k = int(k_match.group(1))
        coefficient = int(result_match.group(3))

        return jsonify({
            "success": True,
            "algorithm": "Binomial Coefficient Dynamic Programming",
            "method": "DP Table / Tabulation",
            "available_teams": n,
            "teams_to_select": k,
            "binomial_coefficient": coefficient,
            "time_complexity": "O(n * k)",
            "space_complexity": "O(n * k)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Binomial execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500