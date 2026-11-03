import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

matrix_chain_bp = Blueprint("matrix_chain", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

MATRIX_CHAIN_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "matrix_chain"
    / "matrix_chain.exe"
)


@matrix_chain_bp.get("/api/v1/algorithms/matrix-chain")
def run_matrix_chain():

    try:
        if not MATRIX_CHAIN_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Matrix Chain executable not found."
            }), 500

        result = subprocess.run(
            [str(MATRIX_CHAIN_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Matrix Chain execution failed."
            }), 500

        output = result.stdout

        # Extract matrix dimensions.
        matrix_matches = re.findall(
            r"(A\d+)\s*=\s*(\d+)\s*x\s*(\d+)",
            output
        )

        matrices = [
            {
                "matrix": name,
                "rows": int(rows),
                "columns": int(columns)
            }
            for name, rows, columns in matrix_matches
        ]

        # Extract minimum multiplication cost.
        cost_match = re.search(
            r"Minimum scalar multiplications\s*=\s*(\d+)",
            output
        )

        if not cost_match or not matrices:
            return jsonify({
                "success": False,
                "message": "Could not extract Matrix Chain result.",
                "output": output
            }), 500

        minimum_cost = int(cost_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Matrix Chain Multiplication",
            "method": "Dynamic Programming",
            "matrix_count": len(matrices),
            "matrices": matrices,
            "minimum_scalar_multiplications": minimum_cost,
            "time_complexity": "O(n^3)",
            "space_complexity": "O(n^2)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Matrix Chain execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500