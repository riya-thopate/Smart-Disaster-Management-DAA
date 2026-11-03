import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

independent_set_bp = Blueprint(
    "independent_set",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

INDEPENDENT_SET_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "independent_set"
    / "independent_set.exe"
)


@independent_set_bp.get("/api/v1/algorithms/independent-set")
def run_independent_set():

    try:
        if not INDEPENDENT_SET_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Independent Set executable not found."
            }), 500

        result = subprocess.run(
            [str(INDEPENDENT_SET_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Execution failed."
            }), 500

        output = result.stdout

        # Extract hierarchy locations safely.
        location_section_match = re.search(
            r"Disaster-response hierarchy:\s*\n"
            r"(.*?)"
            r"Tree connections:",
            output,
            re.DOTALL
        )

        if not location_section_match:
            raise ValueError("Hierarchy section not found.")

        locations = [
            {
                "id": int(location_id),
                "name": name.strip()
            }
            for location_id, name in re.findall(
                r"^\s*(\d+)\s*->\s*(.+?)\s*$",
                location_section_match.group(1),
                re.MULTILINE
            )
        ]

        # Extract tree connections safely.
        connection_section_match = re.search(
            r"Tree connections:\s*\n"
            r"(.*?)"
            r"Maximum Independent Set Size",
            output,
            re.DOTALL
        )

        if not connection_section_match:
            raise ValueError("Tree connections section not found.")

        connections = [
            {
                "source": source.strip(),
                "destination": destination.strip()
            }
            for source, destination in re.findall(
                r"^\s*(.+?)\s*->\s*(.+?)\s*$",
                connection_section_match.group(1),
                re.MULTILINE
            )
        ]

        # Extract maximum independent set size.
        size_match = re.search(
            r"Maximum Independent Set Size\s*=\s*(\d+)",
            output
        )

        if not size_match:
            raise ValueError("Maximum independent set size not found.")

        maximum_set_size = int(size_match.group(1))

        # Extract selected locations safely.
               # Extract selected locations line by line.
        lines = output.splitlines()

        selected_header_index = next(
            (
                i for i, line in enumerate(lines)
                if line.strip() == "Selected locations:"
            ),
            None
        )

        interpretation_index = next(
            (
                i for i, line in enumerate(lines)
                if line.strip() == "Interpretation:"
            ),
            None
        )

        if (
            selected_header_index is None
            or interpretation_index is None
        ):
            raise ValueError(
                "Selected locations section not found."
            )

        selected_locations = []

        for line in lines[
            selected_header_index + 1:interpretation_index
        ]:
            cleaned = line.strip()

            if cleaned.startswith("- "):
                selected_locations.append(
                    cleaned[2:].strip()
                )

        # Validate extracted data.
        if not selected_locations:
            raise ValueError(
                "No selected locations were extracted."
            )

        if len(selected_locations) != maximum_set_size:
            return jsonify({
                "success": False,
                "message": "Selected location count mismatch.",
                "reported_size": maximum_set_size,
                "extracted_count": len(selected_locations),
                "extracted_locations": selected_locations,
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Largest Independent Set",
            "method": "Tree Dynamic Programming",
            "location_count": len(locations),
            "locations": locations,
            "connections": connections,
            "selected_locations": selected_locations,
            "maximum_independent_set_size": maximum_set_size,
            "time_complexity": "O(V)",
            "space_complexity": "O(V)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Independent Set execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500