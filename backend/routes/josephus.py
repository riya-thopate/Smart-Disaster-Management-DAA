import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

josephus_bp = Blueprint("josephus", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

JOSEPHUS_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "josephus"
    / "josephus.exe"
)


@josephus_bp.get("/api/v1/algorithms/josephus")
def run_josephus():

    try:
        if not JOSEPHUS_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Josephus executable not found."
            }), 500

        result = subprocess.run(
            [str(JOSEPHUS_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Josephus execution failed."
            }), 500

        output = result.stdout

        # Extract the emergency team list.
        if "Emergency Response Teams:" not in output:
            return jsonify({
                "success": False,
                "message": "Team information not found.",
                "output": output
            }), 500

        teams_section = output.split(
            "Emergency Response Teams:", 1
        )[1].split(
            "Elimination step:", 1
        )[0]

        team_matches = re.findall(
            r"^\s*Team\s+(\d+)\s*$",
            teams_section,
            re.MULTILINE
        )

        elimination_match = re.search(
            r"Elimination step:\s*(\d+)",
            output
        )

        survivor_match = re.search(
            r"Surviving Team:\s*Team\s+(\d+)",
            output
        )

        if not all([
            team_matches,
            elimination_match,
            survivor_match
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract Josephus result.",
                "output": output
            }), 500

        number_of_teams = len(team_matches)
        elimination_step = int(elimination_match.group(1))
        survivor = int(survivor_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Josephus Problem",
            "number_of_teams": number_of_teams,
            "elimination_step": elimination_step,
            "surviving_team": survivor,
            "time_complexity": "O(n)",
            "space_complexity": "O(n) recursive stack",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Josephus execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500