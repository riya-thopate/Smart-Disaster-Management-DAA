
from pathlib import Path

from flask import Blueprint, request, jsonify
from services.algorithm_runner import run_cpp_algorithm

max_flow_bp = Blueprint("max_flow", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
ROADS_CSV = PROJECT_ROOT / "data" / "roads.csv"


@max_flow_bp.get("/api/v1/flow/max")
def calculate_max_flow():
    source = request.args.get("source", "L10").strip().upper()
    destination = request.args.get("destination", "L7").strip().upper()

    if not source or not destination:
        return jsonify({
            "success": False,
            "message": "Source and destination are required"
        }), 400

    if source == destination:
        return jsonify({
            "success": False,
            "message": "Source and destination must be different"
        }), 400

    try:
        result = run_cpp_algorithm(
            "max_flow_api.exe",
            [
                str(ROADS_CSV),
                source,
                destination
            ]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
