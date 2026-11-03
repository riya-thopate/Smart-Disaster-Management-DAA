import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

tower_of_hanoi_bp = Blueprint(
    "tower_of_hanoi",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

TOWER_OF_HANOI_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit2"
    / "tower_of_hanoi"
    / "tower_of_hanoi.exe"
)


@tower_of_hanoi_bp.get("/api/v1/algorithms/tower-of-hanoi")
def run_tower_of_hanoi():

    try:
        if not TOWER_OF_HANOI_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Tower of Hanoi executable not found."
            }), 500

        result = subprocess.run(
            [str(TOWER_OF_HANOI_EXE)],
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

        disk_match = re.search(
            r"Number of disks:\s*(\d+)",
            output
        )

        total_match = re.search(
            r"Total moves required:\s*(\d+)",
            output
        )

        move_matches = re.findall(
            r"Move disk (\d+) from ([A-Z]) to ([A-Z])",
            output
        )

        if not disk_match or not total_match:
            return jsonify({
                "success": False,
                "message": "Could not extract Tower of Hanoi result.",
                "output": output
            }), 500

        number_of_disks = int(disk_match.group(1))
        total_moves = int(total_match.group(1))

        moves = [
            {
                "disk": int(disk),
                "source": source,
                "destination": destination
            }
            for disk, source, destination in move_matches
        ]

        return jsonify({
            "success": True,
            "algorithm": "Tower of Hanoi",
            "number_of_disks": number_of_disks,
            "rods": {
                "source": "A",
                "auxiliary": "B",
                "destination": "C"
            },
            "total_moves": total_moves,
            "moves": moves,
            "time_complexity": "O(2^n)",
            "space_complexity": "O(n) recursive stack",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Tower of Hanoi execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500