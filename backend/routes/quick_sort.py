import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

quick_sort_bp = Blueprint("quick_sort", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

QUICK_SORT_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "quick_sort"
    / "quick_sort.exe"
)


@quick_sort_bp.get("/api/v1/algorithms/quick-sort")
def run_quick_sort():

    try:
        if not QUICK_SORT_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Quick Sort executable not found."
            }), 500

        result = subprocess.run(
            [str(QUICK_SORT_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Quick Sort execution failed."
            }), 500

        output = result.stdout

        if "After Quick Sort:" not in output:
            return jsonify({
                "success": False,
                "message": "Could not find sorted output.",
                "output": output
            }), 500

        sorted_section = output.split(
            "After Quick Sort:", 1
        )[1].split(
            "Highest priority emergency:", 1
        )[0]

        pattern = (
            r"^\s*(E\d+)\s+\|\s+([^|]+)\s+\|\s+"
            r"([^|]+)\s+\|\s+Affected People:\s*(\d+)\s*$"
        )

        matches = re.findall(
            pattern,
            sorted_section,
            re.MULTILINE
        )

        emergencies = [
            {
                "id": item[0],
                "type": item[1].strip(),
                "location": item[2].strip(),
                "affected_people": int(item[3])
            }
            for item in matches
        ]

        if not emergencies:
            return jsonify({
                "success": False,
                "message": "Could not extract sorted emergency records.",
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Quick Sort",
            "pivot": "Last Element",
            "order": "DESC",
            "sort_field": "affected_people",
            "count": len(emergencies),
            "emergencies": emergencies,
            "highest_priority_emergency": emergencies[0],
            "time_complexity": "O(n log n) average, O(n^2) worst",
            "space_complexity": "O(log n) average recursion stack",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500