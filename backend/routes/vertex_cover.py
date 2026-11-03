from flask import Blueprint, jsonify
from pathlib import Path
import subprocess
import re


vertex_cover_bp = Blueprint("vertex_cover", __name__)


@vertex_cover_bp.route(
    "/api/v1/algorithms/vertex-cover",
    methods=["GET"]
)
def run_vertex_cover():

    executable = (
        Path(__file__).resolve().parent.parent.parent
        / "algorithms"
        / "unit6"
        / "vertex_cover"
        / "vertex_cover.exe"
    )

    if not executable.is_file():
        return jsonify({
            "success": False,
            "message": "Vertex Cover executable not found.",
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
                "message": "Vertex Cover C++ execution failed.",
                "error": process.stderr,
                "output": output
            }), 500

        # Extract locations.
        locations_match = re.search(
            r"Disaster-management locations:\s*(.*?)"
            r"\n\s*Road connections:",
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

        # Extract graph road connections.
        edges_match = re.search(
            r"Road connections:\s*(.*?)"
            r"\n\s*Required vertex cover size",
            output,
            re.S | re.I
        )

        edges = []

        if edges_match:
            for line in edges_match.group(1).splitlines():
                match = re.match(
                    r"\s*(.+?)\s*<->\s*(.+?)\s*$",
                    line
                )

                if match:
                    edges.append({
                        "source": match.group(1).strip(),
                        "destination": match.group(2).strip()
                    })

        # Extract required vertex cover size.
        size_match = re.search(
            r"Required vertex cover size\s*=\s*(\d+)",
            output,
            re.I
        )

        required_size = (
            int(size_match.group(1))
            if size_match
            else 0
        )

        # Check whether a vertex cover was found.
        cover_found = "Vertex Cover Found:" in output

        selected_locations = []

        if cover_found:
            cover_match = re.search(
                r"Vertex Cover Found:\s*(.*?)"
                r"\n\s*-{5,}",
                output,
                re.S | re.I
            )

            if cover_match:
                selected_locations = re.findall(
                    r"^\s*-\s*(.+?)\s*$",
                    cover_match.group(1),
                    re.M
                )

        # Extract C++ verification message.
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
                "No vertex cover found."
                if not cover_found
                else "Verification details unavailable."
            )
        )

        # Independently verify that every edge is covered.
        selected_set = set(selected_locations)

        all_edges_covered = (
            cover_found
            and len(edges) > 0
            and all(
                edge["source"] in selected_set
                or edge["destination"] in selected_set
                for edge in edges
            )
        )

        verified = (
            all_edges_covered
            and len(selected_locations) == required_size
        )

        return jsonify({
            "success": True,
            "algorithm": "Vertex Cover Problem",

            "method": "Backtracking with Edge Coverage Verification",

            "number_of_vertices": len(locations),
            "locations": locations,

            "number_of_edges": len(edges),
            "edges": edges,

            "required_vertex_cover_size": required_size,

            "vertex_cover_found": cover_found,

            "selected_locations": selected_locations,

            "vertex_cover_size": len(selected_locations),

            "all_edges_covered": all_edges_covered,

            "is_vertex_cover_verified": verified,

            "verification": verification,

            "result": (
                "VERTEX COVER FOUND"
                if cover_found
                else "NO VERTEX COVER FOUND"
            ),

            "time_complexity": "O(2^V * E * V)",
            "space_complexity": "O(V)",

            "cpp_output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Vertex Cover execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": "An error occurred while running Vertex Cover.",
            "error": str(error)
        }), 500