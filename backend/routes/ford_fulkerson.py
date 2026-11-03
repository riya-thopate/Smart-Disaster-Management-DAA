import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

ford_fulkerson_bp = Blueprint(
    "ford_fulkerson",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

FORD_FULKERSON_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit5"
    / "ford_fulkerson"
    / "ford_fulkerson.exe"
)


@ford_fulkerson_bp.get(
    "/api/v1/algorithms/ford-fulkerson"
)
def run_ford_fulkerson():

    try:
        if not FORD_FULKERSON_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Ford-Fulkerson executable not found.",
                "expected_path": str(FORD_FULKERSON_EXE)
            }), 500

        result = subprocess.run(
            [str(FORD_FULKERSON_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": (
                    result.stderr or
                    "Ford-Fulkerson execution failed."
                )
            }), 500

        output = result.stdout

        # Extract locations.
        location_section = re.search(
            r"Disaster evacuation network:\s*\n"
            r"(.*?)"
            r"Network capacities:",
            output,
            re.DOTALL
        )

        if not location_section:
            raise ValueError(
                "Disaster evacuation network section not found."
            )

        location_matches = re.findall(
            r"^\s*(\d+)\.\s*(.+?)\s*$",
            location_section.group(1),
            re.MULTILINE
        )

        locations = [
            {
                "id": int(location_id),
                "name": name.strip()
            }
            for location_id, name in location_matches
        ]

        # Extract network edge capacities.
        capacity_section = re.search(
            r"Network capacities:\s*\n"
            r"(.*?)"
            r"Maximum evacuation flow",
            output,
            re.DOTALL
        )

        if not capacity_section:
            raise ValueError(
                "Network capacity section not found."
            )

        edge_matches = re.findall(
            r"^\s*(.+?)\s*->\s*(.+?)\s*:\s*(\d+)\s*$",
            capacity_section.group(1),
            re.MULTILINE
        )

        edges = [
            {
                "source": source.strip(),
                "destination": destination.strip(),
                "capacity": int(capacity)
            }
            for source, destination, capacity in edge_matches
        ]

        # Extract maximum flow.
        flow_match = re.search(
            r"Maximum evacuation flow\s*=\s*(\d+)\s*people",
            output
        )

        if not locations or not edges or not flow_match:
            raise ValueError(
                "Could not extract Ford-Fulkerson results."
            )

        maximum_flow = int(flow_match.group(1))

        return jsonify({
            "success": True,
            "algorithm": "Ford-Fulkerson Maximum Flow",
            "method": "Ford-Fulkerson with BFS augmenting paths",
            "location_count": len(locations),
            "locations": locations,
            "source": locations[0]["name"],
            "destination": locations[-1]["name"],
            "edges": edges,
            "edge_count": len(edges),
            "maximum_flow": maximum_flow,
            "flow_unit": "people",
            "time_complexity": "O(V * E^2) - BFS augmenting paths",
            "space_complexity": "O(V^2)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Ford-Fulkerson execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500