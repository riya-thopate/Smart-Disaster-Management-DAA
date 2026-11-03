import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

order_statistics_bp = Blueprint(
    "order_statistics",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

ORDER_STATISTICS_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "order_statistics"
    / "order_statistics.exe"
)


@order_statistics_bp.get("/api/v1/algorithms/order-statistics")
def calculate_order_statistics():

    try:
        if not ORDER_STATISTICS_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Order Statistics executable not found."
            }), 500

        result = subprocess.run(
            [str(ORDER_STATISTICS_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or
                    "Order Statistics execution failed."
            }), 500

        output = result.stdout

        if "3rd Most Affected Emergency:" not in output:
            return jsonify({
                "success": False,
                "message": "Could not extract the Order Statistics result.",
                "output": output
            }), 500

        result_section = output.split(
            "3rd Most Affected Emergency:", 1
        )[1]

        emergency_id = re.search(
            r"Emergency ID\s*:\s*(.+)",
            result_section
        )

        emergency_type = re.search(
            r"Type\s*:\s*(.+)",
            result_section
        )

        location = re.search(
            r"Location\s*:\s*(.+)",
            result_section
        )

        affected_people = re.search(
            r"Affected People\s*:\s*(\d+)",
            result_section
        )

        if not all([
            emergency_id,
            emergency_type,
            location,
            affected_people
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract emergency details.",
                "output": output
            }), 500

        emergency = {
            "id": emergency_id.group(1).strip(),
            "type": emergency_type.group(1).strip(),
            "location": location.group(1).strip(),
            "affected_people": int(affected_people.group(1))
        }

        return jsonify({
            "success": True,
            "algorithm": "Quickselect - Order Statistics",
            "k": 3,
            "rank": "3rd Highest",
            "emergency": emergency,
            "time_complexity": "O(n) average, O(n^2) worst",
            "space_complexity": "O(log n) average, O(n) worst",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500