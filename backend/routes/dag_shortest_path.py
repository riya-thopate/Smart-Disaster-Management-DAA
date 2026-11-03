import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

dag_shortest_path_bp = Blueprint(
    "dag_shortest_path",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

DAG_SHORTEST_PATH_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "dag_shortest_path"
    / "dag_shortest_path.exe"
)


@dag_shortest_path_bp.get(
    "/api/v1/algorithms/dag-shortest-path"
)
def run_dag_shortest_path():

    try:
        if not DAG_SHORTEST_PATH_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "DAG Shortest Path executable not found."
            }), 500

        result = subprocess.run(
            [str(DAG_SHORTEST_PATH_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "DAG execution failed."
            }), 500

        output = result.stdout

        # Extract disaster-response locations.
        if "Disaster-response locations:" not in output:
            return jsonify({
                "success": False,
                "message": "Location information not found.",
                "output": output
            }), 500

        locations_section = output.split(
            "Disaster-response locations:", 1
        )[1].split(
            "Starting Location:", 1
        )[0]

        location_matches = re.findall(
            r"^\s*(\d+)\s*->\s*(.+?)\s*$",
            locations_section,
            re.MULTILINE
        )

        locations = [
            {
                "id": int(location_id),
                "name": name.strip()
            }
            for location_id, name in location_matches
        ]

        # Extract starting location.
        source_match = re.search(
            r"Starting Location:\s*(.+)",
            output
        )

        # Extract shortest distances.
        if "Shortest distances from Emergency Center:" not in output:
            return jsonify({
                "success": False,
                "message": "Shortest-distance section not found.",
                "output": output
            }), 500

        distance_section = output.split(
            "Shortest distances from Emergency Center:", 1
        )[1].split(
            "\n----------------------------------------", 1
        )[0]

        distance_matches = re.findall(
            r"^\s*(.+?)\s*->\s*(Unreachable|\d+)(?:\s+units)?\s*$",
            distance_section,
            re.MULTILINE
        )

        distances = [
            {
                "location": name.strip(),
                "distance": (
                    int(distance)
                    if distance != "Unreachable"
                    else None
                ),
                "reachable": distance != "Unreachable"
            }
            for name, distance in distance_matches
        ]

        # Extract shortest distance to shelter.
        shelter_match = re.search(
            r"Shortest distance to Shelter\s*=\s*(\d+)\s*units",
            output
        )

        if not all([
            locations,
            source_match,
            distances,
            shelter_match
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract DAG Shortest Path results.",
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "DAG Shortest Path",
            "method": "Topological Sorting + Dynamic Programming",
            "location_count": len(locations),
            "locations": locations,
            "source": source_match.group(1).strip(),
            "destination": "Shelter",
            "distances": distances,
            "shortest_distance_to_shelter": int(
                shelter_match.group(1)
            ),
            "time_complexity": "O(V + E)",
            "space_complexity": "O(V + E)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "DAG Shortest Path execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500