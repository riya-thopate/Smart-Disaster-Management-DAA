
from pathlib import Path

from flask import Blueprint, jsonify
from services.algorithm_runner import run_cpp_algorithm

rescue_matching_bp = Blueprint(
    "rescue_matching",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"


@rescue_matching_bp.get("/api/v1/rescue-teams/match")
def match_rescue_teams():
    emergencies_csv = DATA_DIR / "emergencies.csv"
    teams_csv = DATA_DIR / "rescue_teams.csv"

    try:
        result = run_cpp_algorithm(
            "rescue_matching_api.exe",
            [
                str(emergencies_csv),
                str(teams_csv)
            ]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
