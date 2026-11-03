import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

fast_modular_bp = Blueprint(
    "fast_modular",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

FAST_MODULAR_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "fast_modular_exponentiation"
    / "fast_modular_exponentiation.exe"
)


@fast_modular_bp.get(
    "/api/v1/algorithms/fast-modular-exponentiation"
)
def run_fast_modular_exponentiation():

    try:
        if not FAST_MODULAR_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Fast Modular Exponentiation executable not found."
            }), 500

        result = subprocess.run(
            [str(FAST_MODULAR_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Algorithm execution failed."
            }), 500

        output = result.stdout

        calculation_match = re.search(
            r"(\d+)\^(\d+)\s+mod\s+(\d+)",
            output
        )

        result_match = re.search(
            r"Result:\s*(\d+)",
            output
        )

        if not calculation_match or not result_match:
            return jsonify({
                "success": False,
                "message": "Could not extract calculation result.",
                "output": output
            }), 500

        base = int(calculation_match.group(1))
        exponent = int(calculation_match.group(2))
        modulus = int(calculation_match.group(3))
        answer = int(result_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Fast Modular Exponentiation",
            "base": base,
            "exponent": exponent,
            "modulus": modulus,
            "result": answer,
            "time_complexity": "O(log exponent)",
            "space_complexity": "O(1) auxiliary space (iterative)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Algorithm execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500