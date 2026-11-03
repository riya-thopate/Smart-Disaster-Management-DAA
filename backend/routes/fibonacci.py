import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

fibonacci_bp = Blueprint("fibonacci", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

FIBONACCI_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "fibonacci"
    / "fibonacci.exe"
)


@fibonacci_bp.get("/api/v1/algorithms/fibonacci")
def run_fibonacci():

    try:
        if not FIBONACCI_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Fibonacci executable not found."
            }), 500

        result = subprocess.run(
            [str(FIBONACCI_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Fibonacci execution failed."
            }), 500

        output = result.stdout

        n_match = re.search(
            r"Number of disaster-response stages:\s*(\d+)",
            output
        )

        fibonacci_match = re.search(
            r"Fibonacci\((\d+)\)\s*=\s*(\d+)",
            output
        )

        if not n_match or not fibonacci_match:
            return jsonify({
                "success": False,
                "message": "Could not extract Fibonacci result.",
                "output": output
            }), 500

        n = int(n_match.group(1))
        fibonacci_value = int(fibonacci_match.group(2))

        dp_matches = re.findall(
            r"F\[(\d+)\]\s*=\s*(\d+)",
            output
        )

        dp_table = [
            {
                "index": int(index),
                "value": int(value)
            }
            for index, value in dp_matches
        ]

        return jsonify({
            "success": True,
            "algorithm": "Fibonacci Dynamic Programming",
            "method": "Bottom-Up Tabulation",
            "n": n,
            "fibonacci_value": fibonacci_value,
            "dp_table": dp_table,
            "time_complexity": "O(n)",
            "space_complexity": "O(n)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Fibonacci execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500