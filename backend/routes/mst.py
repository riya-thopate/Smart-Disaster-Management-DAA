
from pathlib import Path

from flask import Blueprint, jsonify
from services.algorithm_runner import run_cpp_algorithm

mst_bp = Blueprint("mst", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
ROADS_CSV = PROJECT_ROOT / "data" / "roads.csv"


@mst_bp.get("/api/v1/graph/mst")
def calculate_mst():
    try:
        result = run_cpp_algorithm(
            "mst_api.exe",
            [str(ROADS_CSV)]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
