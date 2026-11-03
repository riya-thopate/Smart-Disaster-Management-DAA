from flask import Blueprint, jsonify
from pathlib import Path
import subprocess
import re


clique_bp = Blueprint("clique", __name__)


@clique_bp.route("/api/v1/algorithms/clique", methods=["GET"])
def run_clique():

    executable = (
        Path(__file__).resolve().parent.parent.parent
        / "algorithms"
        / "unit6"
        / "clique"
        / "clique.exe"
    )

    if not executable.is_file():
        return jsonify({
            "success": False,
            "message": "Clique executable not found.",
            "expected_path": str(executable)
        }), 404

    try:
        process = subprocess.run(
            [str(executable)],
            capture_output=True,
            text=True,
            timeout=10,
            cwd=str(executable.parent)
        )

        output = process.stdout or ""

        if process.returncode != 0:
            return jsonify({
                "success": False,
                "message": "Clique C++ program execution failed.",
                "error": process.stderr,
                "output": output
            }), 500

        # Extract all graph locations.
        locations_match = re.search(
            r"Disaster-management locations:\s*(.*?)"
            r"\n\s*Required clique size",
            output,
            re.S | re.I
        )

        locations = []

        if locations_match:
            locations = re.findall(
                r"^\s*\d+\.\s*(.+?)\s*$",
                locations_match.group(1),
                re.M
            )

        # Extract the required clique size.
        size_match = re.search(
            r"Required clique size\s*=\s*(\d+)",
            output,
            re.I
        )

        required_size = (
            int(size_match.group(1))
            if size_match
            else 0
        )

        # Determine whether a clique was found.
        clique_found = "Clique found:" in output

        selected_locations = []

        if clique_found:
            clique_match = re.search(
                r"Clique found:\s*(.*?)"
                r"\n\s*-{5,}",
                output,
                re.S | re.I
            )

            if clique_match:
                selected_locations = re.findall(
                    r"^\s*-\s*(.+?)\s*$",
                    clique_match.group(1),
                    re.M
                )

        # Extract the verification explanation.
        verification_match = re.search(
            r"Verification:\s*(.*?)"
            r"\n\s*-{5,}",
            output,
            re.S | re.I
        )

        verification = (
            " ".join(verification_match.group(1).split())
            if verification_match
            else (
                "No clique exists for the required size."
                if not clique_found
                else "Clique verification details unavailable."
            )
        )

        return jsonify({
            "success": True,
            "algorithm": "Clique Problem",

            "method": "Backtracking with Subset Verification",

            "number_of_vertices": len(locations),
            "locations": locations,

            "required_clique_size": required_size,

            "clique_found": clique_found,

            "selected_locations": selected_locations,

            "clique_size": len(selected_locations),

            "is_clique_verified": (
                clique_found
                and len(selected_locations) == required_size
            ),

            "verification": verification,

            "result": (
                "CLIQUE FOUND"
                if clique_found
                else "NO CLIQUE FOUND"
            ),

            "time_complexity": "O(2^n * n^2)",
            "space_complexity": "O(n)",

            "cpp_output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Clique execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": "An error occurred while running Clique.",
            "error": str(error)
        }), 500