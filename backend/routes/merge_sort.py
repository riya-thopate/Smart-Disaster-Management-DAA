import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

merge_sort_bp = Blueprint("merge_sort", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

MERGE_SORT_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "merge_sort"
    / "merge_sort.exe"
)


@merge_sort_bp.get("/api/v1/algorithms/merge-sort")
def run_merge_sort():

    try:
        if not MERGE_SORT_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Merge Sort executable not found."
            }), 500

        result = subprocess.run(
            [str(MERGE_SORT_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Merge Sort execution failed."
            }), 500

        output = result.stdout

        if "After Merge Sort:" not in output:
            return jsonify({
                "success": False,
                "message": "Could not find sorted output.",
                "output": output
            }), 500

        sorted_section = output.split(
            "After Merge Sort:", 1
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

        return jsonify({
            "success": True,
            "algorithm": "Merge Sort",
            "order": "DESC",
            "sort_field": "affected_people",
            "count": len(emergencies),
            "emergencies": emergencies,
            "highest_priority_emergency": (
                emergencies[0] if emergencies else None
            ),
            "time_complexity": "O(n log n)",
            "space_complexity": "O(n)",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500