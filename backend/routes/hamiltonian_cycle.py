import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

hamiltonian_cycle_bp = Blueprint(
    "hamiltonian_cycle",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

HAMILTONIAN_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit4"
    / "backtracking"
    / "hamiltonian_cycle.exe"
)


@hamiltonian_cycle_bp.get(
    "/api/v1/algorithms/hamiltonian-cycle"
)
def run_hamiltonian_cycle():

    try:
        if not HAMILTONIAN_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Hamiltonian Cycle executable not found.",
                "expected_path": str(HAMILTONIAN_EXE)
            }), 500

        result = subprocess.run(
            [str(HAMILTONIAN_EXE)],
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
                    "Hamiltonian Cycle execution failed."
                )
            }), 500

        output = result.stdout

        # Extract selected locations.
        location_section_match = re.search(
            r"Selected disaster-management locations:\s*\n"
            r"(.*?)"
            r"Starting Location:",
            output,
            re.DOTALL
        )

        if not location_section_match:
            raise ValueError(
                "Selected locations section not found."
            )

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

        # Extract starting location.
        source_match = re.search(
            r"Starting Location:\s*\n\s*(.+)",
            output
        )

        if not locations or not source_match:
            raise ValueError(
                "Could not extract location information."
            )

        # Check whether a Hamiltonian cycle was found.
        if "Hamiltonian Cycle Found:" in output:

            cycle_section = output.split(
                "Hamiltonian Cycle Found:", 1
            )[1].split(
                "Every selected location is visited", 1
            )[0]

            cycle_line = next(
                (
                    line.strip()
                    for line in cycle_section.splitlines()
                    if "->" in line
                ),
                None
            )

            if not cycle_line:
                raise ValueError(
                    "Hamiltonian cycle route not found."
                )

            cycle = [
                location.strip()
                for location in cycle_line.split("->")
            ]

            if len(cycle) != len(locations) + 1:
                raise ValueError(
                    "Cycle does not contain the expected number of vertices."
                )

            if cycle[0] != cycle[-1]:
                raise ValueError(
                    "Cycle does not return to its starting location."
                )

            if set(cycle[:-1]) != {
                location["name"] for location in locations
            }:
                raise ValueError(
                    "Cycle does not visit every location exactly once."
                )

            cycle_found = True

        elif "No Hamiltonian Cycle exists." in output:
            cycle = []
            cycle_found = False

        else:
            raise ValueError(
                "Hamiltonian Cycle result not found."
            )

        return jsonify({
            "success": True,
            "algorithm": "Hamiltonian Cycle",
            "method": "Backtracking",
            "location_count": len(locations),
            "locations": locations,
            "starting_location": source_match.group(1).strip(),
            "cycle_found": cycle_found,
            "cycle": cycle,
            "time_complexity": "O(V!)",
            "space_complexity": "O(V) auxiliary; O(V^2) including adjacency matrix",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Hamiltonian Cycle execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500