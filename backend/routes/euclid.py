import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

euclid_bp = Blueprint("euclid", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

EUCLID_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "euclid"
    / "euclid.exe"
)


@euclid_bp.get("/api/v1/algorithms/euclid")
def calculate_euclid():

    try:
        if not EUCLID_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Euclid executable not found."
            }), 500

        result = subprocess.run(
            [str(EUCLID_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Euclid execution failed."
            }), 500

        output = result.stdout

        water_match = re.search(
            r"Water\s*:\s*(\d+)\s*units",
            output
        )

        food_match = re.search(
            r"Food\s*:\s*(\d+)\s*units",
            output
        )

        gcd_match = re.search(
            r"GCD of resource quantities:\s*(\d+)",
            output
        )

        if not all([water_match, food_match, gcd_match]):
            return jsonify({
                "success": False,
                "message": "Could not extract GCD result.",
                "output": output
            }), 500

        water = int(water_match.group(1))
        food = int(food_match.group(1))
        gcd_value = int(gcd_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Euclid's GCD Algorithm",
            "water_units": water,
            "food_units": food,
            "gcd": gcd_value,
            "time_complexity": "O(log(min(a, b)))",
            "space_complexity": "O(log(min(a, b)))",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500