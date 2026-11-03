import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

optimal_bst_bp = Blueprint("optimal_bst", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

OPTIMAL_BST_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "optimal_bst"
    / "optimal_bst.exe"
)

LOCATIONS = [
    {"name": "Hospital", "frequency": 40},
    {"name": "Shelter", "frequency": 30},
    {"name": "Emergency Center", "frequency": 20},
    {"name": "Industrial Area", "frequency": 10},
]


@optimal_bst_bp.get("/api/v1/algorithms/optimal-bst")
def calculate_optimal_bst():
    try:
        if not OPTIMAL_BST_EXE.exists():
            return jsonify({
                "success": False,
                "message": "Optimal BST executable not found."
            }), 500

        result = subprocess.run(
            [str(OPTIMAL_BST_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Optimal BST execution failed."
            }), 500

        match = re.search(
            r"Minimum weighted search cost\s*=\s*(\d+)",
            result.stdout
        )

        if not match:
            return jsonify({
                "success": False,
                "message": "Could not extract minimum search cost.",
                "output": result.stdout
            }), 500

        minimum_cost = int(match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Optimal Binary Search Tree",
            "locations": LOCATIONS,
            "location_count": len(LOCATIONS),
            "minimum_weighted_search_cost": minimum_cost,
            "time_complexity": "O(n^3)",
            "space_complexity": "O(n^2)",
            "output": result.stdout
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500