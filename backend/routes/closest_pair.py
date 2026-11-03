import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

closest_pair_bp = Blueprint("closest_pair", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

CLOSEST_PAIR_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "closest_pair"
    / "closest_pair.exe"
)


@closest_pair_bp.get("/api/v1/algorithms/closest-pair")
def calculate_closest_pair():

    try:
        if not CLOSEST_PAIR_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Closest Pair executable not found."
            }), 500

        result = subprocess.run(
            [str(CLOSEST_PAIR_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Closest Pair execution failed."
            }), 500

        output = result.stdout

        # Extract emergency location coordinates
        coordinate_pattern = (
            r"^\s*(.+?)\s*->\s*"
            r"\(\s*(-?\d+(?:\.\d+)?)\s*,\s*"
            r"(-?\d+(?:\.\d+)?)\s*\)\s*$"
        )

        coordinates = re.findall(
            coordinate_pattern,
            output,
            re.MULTILINE
        )

        points = [
            {
                "name": name.strip(),
                "x": float(x),
                "y": float(y)
            }
            for name, x, y in coordinates
        ]

        # Extract closest pair
        pair_match = re.search(
            r"Closest Pair:\s*-+\s*(.+?)\s*<->\s*(.+?)\s*\n",
            output
        )

        # Extract minimum distance
        distance_match = re.search(
            r"Distance:\s*(\d+(?:\.\d+)?)\s*units",
            output
        )

        if not pair_match or not distance_match:
            return jsonify({
                "success": False,
                "message": "Could not extract closest pair result.",
                "output": output
            }), 500

        closest_pair = [
            pair_match.group(1).strip(),
            pair_match.group(2).strip()
        ]

        minimum_distance = float(distance_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Closest Pair of Points",
            "points": points,
            "point_count": len(points),
            "closest_pair": closest_pair,
            "minimum_distance": minimum_distance,
            "distance_unit": "units",
            "time_complexity": "O(n log^2 n)",
            "space_complexity": "O(n)",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500