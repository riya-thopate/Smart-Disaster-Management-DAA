
from pathlib import Path

from flask import Blueprint, jsonify
from services.algorithm_runner import run_cpp_algorithm

topological_sort_bp = Blueprint(
    "topological_sort",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
TASKS_CSV = PROJECT_ROOT / "data" / "tasks.csv"


@topological_sort_bp.get("/api/v1/algorithms/topological-sort")
def calculate_topological_sort():
    try:
        result = run_cpp_algorithm(
            "topological_sort_api.exe",
            [str(TASKS_CSV)]
        )

        if result.get("cycle_detected"):
            return jsonify(result), 409

        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
