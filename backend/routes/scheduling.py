
from pathlib import Path

from flask import Blueprint, jsonify
from services.algorithm_runner import run_cpp_algorithm

scheduling_bp = Blueprint("scheduling", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
JOBS_CSV = PROJECT_ROOT / "data" / "scheduling_jobs.csv"


@scheduling_bp.get("/api/v1/algorithms/scheduling")
def calculate_scheduling():
    try:
        result = run_cpp_algorithm(
            "scheduling_api.exe",
            [str(JOBS_CSV)]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
