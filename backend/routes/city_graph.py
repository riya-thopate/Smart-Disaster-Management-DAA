import json
import subprocess
from pathlib import Path

from flask import Blueprint, request, jsonify


city_graph_bp = Blueprint(
    "city_graph",
    __name__
)


@city_graph_bp.route(
    "/api/v1/graph/traverse",
    methods=["POST"]
)
def traverse_graph():

    data = request.get_json(silent=True) or {}

    source = str(
        data.get("source", "")
    ).strip().upper()

    algorithm = str(
        data.get("algorithm", "")
    ).strip().upper()

    if not source:
        return jsonify({
            "success": False,
            "message": "Source location is required"
        }), 400

    if algorithm not in {"BFS", "DFS"}:
        return jsonify({
            "success": False,
            "message": "Algorithm must be BFS or DFS"
        }), 400

    backend_dir = (
        Path(__file__).resolve().parent.parent
    )

    executable_name = (
        "bfs_api.exe"
        if algorithm == "BFS"
        else "dfs_api.exe"
    )

    executable = (
        backend_dir
        / "cpp_bin"
        / executable_name
    )

    if not executable.exists():

        return jsonify({
            "success": False,
            "message": f"{executable_name} not found",
            "expected_path": str(executable)
        }), 500

    try:

        result = subprocess.run(
            [
                str(executable),
                source
            ],
            capture_output=True,
            text=True,
            timeout=10
        )

    except subprocess.TimeoutExpired:

        return jsonify({
            "success": False,
            "message": "Algorithm execution timed out"
        }), 500

    if result.returncode != 0:

        return jsonify({
            "success": False,
            "message": "Algorithm execution failed",
            "error": result.stderr
        }), 500

    try:

        response = json.loads(
            result.stdout
        )

    except json.JSONDecodeError:

        return jsonify({
            "success": False,
            "message": "Invalid JSON returned by C++ algorithm",
            "raw_output": result.stdout
        }), 500

    if response.get("success") is False:

        return jsonify(response), 400

    return jsonify(response), 200