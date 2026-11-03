import json
import subprocess
from pathlib import Path

from flask import Blueprint, request, jsonify


evacuation_bp = Blueprint(
    "evacuation",
    __name__
)


@evacuation_bp.route(
    "/api/v1/evacuation/route",
    methods=["POST"]
)
def calculate_route():

    data = request.get_json(silent=True) or {}

    source = str(
        data.get("source", "")
    ).strip().upper()

    destination = str(
        data.get("destination", "")
    ).strip().upper()

    if not source or not destination:
        return jsonify({
            "success": False,
            "message": "Source and destination are required"
        }), 400

    backend_dir = (
        Path(__file__).resolve().parent.parent
    )

    executable = (
        backend_dir
        / "cpp_bin"
        / "dijkstra_api.exe"
    )

    if not executable.exists():
        return jsonify({
            "success": False,
            "message": "Dijkstra executable not found",
            "expected_path": str(executable)
        }), 500

    try:

        result = subprocess.run(
            [
                str(executable),
                source,
                destination
            ],
            capture_output=True,
            text=True,
            timeout=10
        )

    except subprocess.TimeoutExpired:

        return jsonify({
            "success": False,
            "message": "Dijkstra execution timed out"
        }), 500

    if result.returncode != 0:

        return jsonify({
            "success": False,
            "message": "Dijkstra execution failed",
            "error": result.stderr
        }), 500

    try:

        response = json.loads(
            result.stdout
        )

    except json.JSONDecodeError:

        return jsonify({
            "success": False,
            "message": "Invalid JSON returned by Dijkstra",
            "raw_output": result.stdout
        }), 500

    if response.get("success") is False:

        return jsonify(response), 400

    return jsonify(response), 200