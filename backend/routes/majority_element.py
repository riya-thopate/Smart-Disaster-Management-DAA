import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

majority_element_bp = Blueprint(
    "majority_element",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

MAJORITY_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "majority_element"
    / "majority_element.exe"
)


@majority_element_bp.get("/api/v1/algorithms/majority-element")
def calculate_majority_element():

    try:
        if not MAJORITY_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Majority Element executable not found."
            }), 500

        result = subprocess.run(
            [str(MAJORITY_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or
                    "Majority Element execution failed."
            }), 500

        output = result.stdout

        match = re.search(
            r"Majority emergency type found:\s*([A-Z_]+)",
            output
        )

        has_majority = match is not None
        majority_type = match.group(1) if match else "NONE"

        return jsonify({
            "success": True,
            "algorithm": "Boyer-Moore Majority Vote",
            "has_majority": has_majority,
            "majority_type": majority_type,
            "time_complexity": "O(n)",
            "space_complexity": "O(1)",
            "output": output
        })

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500