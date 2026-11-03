import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

graph_coloring_bp = Blueprint(
    "graph_coloring",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

GRAPH_COLORING_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit4"
    / "backtracking"
    / "graph_coloring.exe"
)


@graph_coloring_bp.get("/api/v1/algorithms/graph-coloring")
def run_graph_coloring():

    try:
        if not GRAPH_COLORING_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Graph Coloring executable not found.",
                "expected_path": str(GRAPH_COLORING_EXE)
            }), 500

        result = subprocess.run(
            [str(GRAPH_COLORING_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Graph Coloring execution failed."
            }), 500

        output = result.stdout

        # Extract locations.
        location_section_match = re.search(
            r"Disaster-management locations:\s*\n"
            r"(.*?)"
            r"Number of available colors",
            output,
            re.DOTALL
        )

        if not location_section_match:
            return jsonify({
                "success": False,
                "message": "Location section not found.",
                "output": output
            }), 500

        location_matches = re.findall(
            r"^\s*(\d+)\.\s*(.+?)\s*$",
            location_section_match.group(1),
            re.MULTILINE
        )

        locations = [
            {
                "id": int(location_id),
                "name": name.strip()
            }
            for location_id, name in location_matches
        ]

        # Extract available color count.
        colors_match = re.search(
            r"Number of available colors\s*=\s*(\d+)",
            output
        )

        # Extract coloring result.
        coloring_found = "Valid coloring found:" in output

        coloring_impossible = re.search(
            r"No valid coloring exists with\s+(\d+)\s+colors",
            output
        )

        color_assignments = []

        if coloring_found:
            coloring_section = output.split(
                "Valid coloring found:", 1
            )[1].split(
                "All directly connected locations", 1
            )[0]

            assignment_matches = re.findall(
                r"^\s*(.+?)\s*->\s*Color\s+(\d+)\s*$",
                coloring_section,
                re.MULTILINE
            )

            color_assignments = [
                {
                    "location": name.strip(),
                    "color": int(color)
                }
                for name, color in assignment_matches
            ]

        if not locations or not colors_match:
            return jsonify({
                "success": False,
                "message": "Could not extract Graph Coloring information.",
                "output": output
            }), 500

        total_colors = int(colors_match.group(1))

        if coloring_found:
            if len(color_assignments) != len(locations):
                return jsonify({
                    "success": False,
                    "message": "Color assignment count does not match location count.",
                    "output": output
                }), 500

            coloring_possible = True

        elif coloring_impossible:
            coloring_possible = False

        else:
            return jsonify({
                "success": False,
                "message": "Graph Coloring result not found.",
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Graph Coloring",
            "method": "Backtracking",
            "location_count": len(locations),
            "locations": locations,
            "total_colors": total_colors,
            "coloring_possible": coloring_possible,
            "color_assignments": color_assignments,
            "time_complexity": "O(m^V * V)",
            "space_complexity": "O(V^2)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Graph Coloring execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500