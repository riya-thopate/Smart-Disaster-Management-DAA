import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

lis_bp = Blueprint("lis", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

LIS_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "lis"
    / "lis.exe"
)


@lis_bp.get("/api/v1/algorithms/lis")
def run_lis():

    try:
        if not LIS_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "LIS executable not found."
            }), 500

        result = subprocess.run(
            [str(LIS_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "LIS execution failed."
            }), 500

        output = result.stdout

        # Extract original priority sequence.
        if "Emergency priority sequence:" not in output:
            return jsonify({
                "success": False,
                "message": "Priority sequence not found.",
                "output": output
            }), 500

        sequence_section = output.split(
            "Emergency priority sequence:", 1
        )[1].split(
            "DP Table:", 1
        )[0]

        priority_sequence = [
            int(value)
            for value in re.findall(r"\d+", sequence_section)
        ]

        # Extract DP table.
        dp_matches = re.findall(
            r"Priority\s+(\d+)\s*->\s*LIS length\s*=\s*(\d+)",
            output
        )

        dp_table = [
            {
                "priority": int(priority),
                "lis_length": int(length)
            }
            for priority, length in dp_matches
        ]

        # Extract reconstructed LIS.
        if "Longest Increasing Subsequence:" not in output:
            return jsonify({
                "success": False,
                "message": "LIS sequence section not found.",
                "output": output
            }), 500

        lis_section = output.split(
            "Longest Increasing Subsequence:", 1
        )[1].split(
            "LIS Length", 1
        )[0]

        lis_sequence = [
            int(value)
            for value in re.findall(r"\d+", lis_section)
        ]

        length_match = re.search(
            r"LIS Length\s*=\s*(\d+)",
            output
        )

        if not all([
            priority_sequence,
            dp_table,
            lis_sequence,
            length_match
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract LIS results.",
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Longest Increasing Subsequence",
            "method": "Dynamic Programming",
            "priority_sequence": priority_sequence,
            "dp_table": dp_table,
            "lis_sequence": lis_sequence,
            "lis_length": int(length_match.group(1)),
            "time_complexity": "O(n^2)",
            "space_complexity": "O(n)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "LIS execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500