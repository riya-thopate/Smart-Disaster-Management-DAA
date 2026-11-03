import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

karatsuba_bp = Blueprint("karatsuba", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

KARATSUBA_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "karatsuba"
    / "karatsuba.exe"
)


@karatsuba_bp.get("/api/v1/algorithms/karatsuba")
def run_karatsuba():

    try:
        if not KARATSUBA_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Karatsuba executable not found."
            }), 500

        result = subprocess.run(
            [str(KARATSUBA_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Karatsuba execution failed."
            }), 500

        output = result.stdout

        number1_match = re.search(
            r"Number 1:\s*(\d+)",
            output
        )

        number2_match = re.search(
            r"Number 2:\s*(\d+)",
            output
        )

        result_match = re.search(
            r"Karatsuba Result:\s*(\d+)",
            output
        )

        if not all([
            number1_match,
            number2_match,
            result_match
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract Karatsuba result.",
                "output": output
            }), 500

        number1 = int(number1_match.group(1))
        number2 = int(number2_match.group(1))
        product = int(result_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Karatsuba Multiplication",
            "number1": number1,
            "number2": number2,
            "result": product,
            "time_complexity": "O(n^1.585)",
            "space_complexity": "O(log n) recursive stack",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500