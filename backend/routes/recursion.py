import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

recursion_bp = Blueprint("recursion", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

RECURSION_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "recursion"
    / "recursion.exe"
)


@recursion_bp.get("/api/v1/algorithms/recursion")
def run_recursion():

    try:
        if not RECURSION_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Recursion executable not found."
            }), 500

        result = subprocess.run(
            [str(RECURSION_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Recursion execution failed."
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Recursive Disaster Zone Exploration",
            "root": "Disaster Management Zone",
            "node_count": 10,
            "time_complexity": "O(N)",
            "space_complexity": "O(H)",
            "output": result.stdout.strip()
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500