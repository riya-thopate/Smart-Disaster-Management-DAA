import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

randomized_bp = Blueprint("randomized", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

RANDOMIZED_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "randomized"
    / "randomized.exe"
)


@randomized_bp.get("/api/v1/algorithms/randomized")
def run_randomized():

    try:
        if not RANDOMIZED_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Randomized Quick Sort executable not found."
            }), 500

        result = subprocess.run(
            [str(RANDOMIZED_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Algorithm execution failed."
            }), 500

        output = result.stdout

        # Extract only the sorted emergency records.
        if "Sorted by affected people" not in output:
            return jsonify({
                "success": False,
                "message": "Sorted emergency section not found.",
                "output": output
            }), 500

        sorted_section = output.split(
            "Sorted by affected people", 1
        )[1]

        pattern = (
            r"^\s*(E\d+)\s*\|\s*([^|]+)\s*\|\s*([^|]+)"
            r"\s*\|\s*Affected People:\s*(\d+)\s*$"
        )

        matches = re.findall(
            pattern,
            sorted_section,
            re.MULTILINE
        )

        emergencies = [
            {
                "id": match[0].strip(),
                "type": match[1].strip(),
                "location": match[2].strip(),
                "affected_people": int(match[3])
            }
            for match in matches
        ]

        if not emergencies:
            return jsonify({
                "success": False,
                "message": "Could not extract sorted emergencies.",
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Randomized Quick Sort",
            "pivot_strategy": "Random Pivot",
            "sort_order": "Descending",
            "sort_field": "affected_people",
            "emergency_count": len(emergencies),
            "sorted_emergencies": emergencies,
            "highest_priority": emergencies[0],
            "time_complexity": "O(n log n) average, O(n^2) worst",
            "space_complexity": "O(log n) average, O(n) worst",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Randomized Quick Sort execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500