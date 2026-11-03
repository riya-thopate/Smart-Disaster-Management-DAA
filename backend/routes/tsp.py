
from pathlib import Path

from flask import Blueprint, request, jsonify
from services.algorithm_runner import run_cpp_algorithm

tsp_bp = Blueprint("tsp", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"


@tsp_bp.get("/api/v1/algorithms/tsp")
def calculate_tsp():
    start = request.args.get("start", "L8").strip().upper()

    if not start:
        return jsonify({
            "success": False,
            "message": "Start location is required"
        }), 400

    try:
        result = run_cpp_algorithm(
            "tsp_api.exe",
            [
                str(DATA_DIR / "roads.csv"),
                str(DATA_DIR / "emergencies.csv"),
                str(DATA_DIR / "locations.csv"),
                start
            ],
            timeout=30
        )

        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
