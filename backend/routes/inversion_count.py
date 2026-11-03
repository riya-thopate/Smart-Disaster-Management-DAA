import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

inversion_count_bp = Blueprint(
    "inversion_count",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

INVERSION_COUNT_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "inversion_count"
    / "inversion_count.exe"
)


def extract_emergencies(section):
    pattern = (
        r"(E\d+)\s*\|\s*Affected People:\s*(\d+)"
    )

    matches = re.findall(pattern, section)

    return [
        {
            "id": emergency_id,
            "affected_people": int(people)
        }
        for emergency_id, people in matches
    ]


@inversion_count_bp.get(
    "/api/v1/algorithms/inversion-count"
)
def run_inversion_count():

    try:
        if not INVERSION_COUNT_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Inversion Count executable not found."
            }), 500

        result = subprocess.run(
            [str(INVERSION_COUNT_EXE)],
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

        # Extract original emergency records.
        if "Original Emergency Priority List:" not in output:
            return jsonify({
                "success": False,
                "message": "Original emergency list not found.",
                "output": output
            }), 500

        original_section = output.split(
            "Original Emergency Priority List:", 1
        )[1].split(
            "Sorted Emergency List:", 1
        )[0]

        # Extract sorted emergency records.
        sorted_section = output.split(
            "Sorted Emergency List:", 1
        )[1].split(
            "Total Inversions:", 1
        )[0]

        original_emergencies = extract_emergencies(
            original_section
        )

        sorted_emergencies = extract_emergencies(
            sorted_section
        )

        inversion_match = re.search(
            r"Total Inversions:\s*(\d+)",
            output
        )

        if not inversion_match:
            return jsonify({
                "success": False,
                "message": "Inversion count not found.",
                "output": output
            }), 500

        inversion_count = int(inversion_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Merge Sort - Inversion Count",
            "original_emergencies": original_emergencies,
            "sorted_emergencies": sorted_emergencies,
            "emergency_count": len(original_emergencies),
            "inversion_count": inversion_count,
            "time_complexity": "O(n log n)",
            "space_complexity": "O(n)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Inversion Count execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500