
import math
from pathlib import Path

from flask import Blueprint, request, jsonify
from services.algorithm_runner import run_cpp_algorithm

fractional_knapsack_bp = Blueprint(
    "fractional_knapsack",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
RESOURCES_CSV = PROJECT_ROOT / "data" / "resources.csv"

ALLOWED_TYPES = {"WATER", "FOOD", "MEDICINE", "BLANKET"}


@fractional_knapsack_bp.get("/api/v1/resources/allocate")
def allocate_resources():
    resource_type = request.args.get(
        "resource_type", "WATER"
    ).strip().upper()

    if resource_type not in ALLOWED_TYPES:
        return jsonify({
            "success": False,
            "message": (
                "resource_type must be WATER, FOOD, "
                "MEDICINE, or BLANKET"
            )
        }), 400

    try:
        capacity = float(request.args.get("capacity", "1200"))

        if not math.isfinite(capacity) or capacity <= 0:
            return jsonify({
                "success": False,
                "message": "capacity must be a positive finite number"
            }), 400

    except (ValueError, TypeError):
        return jsonify({
            "success": False,
            "message": "capacity must be a valid number"
        }), 400

    try:
        result = run_cpp_algorithm(
            "fractional_knapsack_api.exe",
            [
                str(RESOURCES_CSV),
                resource_type,
                str(capacity)
            ]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
