import csv
import json
import subprocess
from pathlib import Path

from flask import Blueprint, request, jsonify


emergencies_bp = Blueprint(
    "emergencies",
    __name__
)


@emergencies_bp.route(
    "/api/v1/emergencies",
    methods=["GET"]
)
def get_emergencies():

    backend_dir = (
        Path(__file__).resolve().parent.parent
    )

    csv_path = (
        backend_dir.parent
        / "data"
        / "emergencies.csv"
    )

    if not csv_path.exists():
        return jsonify({
            "success": False,
            "message": "emergencies.csv not found"
        }), 500

    emergencies = []

    try:

        with open(
            csv_path,
            newline="",
            encoding="utf-8"
        ) as file:

            reader = csv.DictReader(file)

            for row in reader:
                emergencies.append({
                    "id": row["emergency_id"],
                    "location_id": row["location_id"],
                    "type": row["type"],
                    "severity": row["severity"],
                    "affectedPeople":
                        int(row["affected_people"]),
                    "resource":
                        row["required_resource"],
                    "status": row["status"]
                })

    except Exception as error:

        return jsonify({
            "success": False,
            "message": str(error)
        }), 500

    return jsonify({
        "success": True,
        "count": len(emergencies),
        "emergencies": emergencies
    })


@emergencies_bp.route(
    "/api/v1/emergencies/sort",
    methods=["GET"]
)
def sort_emergencies():

    order = (
        request.args
        .get("order", "DESC")
        .upper()
    )

    if order not in {"ASC", "DESC"}:

        return jsonify({
            "success": False,
            "message": "order must be ASC or DESC"
        }), 400

    backend_dir = (
        Path(__file__).resolve().parent.parent
    )

    executable = (
        backend_dir
        / "cpp_bin"
        / "emergency_sort_api.exe"
    )

    if not executable.exists():

        return jsonify({
            "success": False,
            "message": "Emergency sort executable not found"
        }), 500

    try:

        result = subprocess.run(
            [
                str(executable),
                order
            ],
            capture_output=True,
            text=True,
            timeout=10
        )

    except subprocess.TimeoutExpired:

        return jsonify({
            "success": False,
            "message": "Emergency sorting timed out"
        }), 500

    if result.returncode != 0:

        return jsonify({
            "success": False,
            "message": "Emergency sorting failed",
            "error": result.stderr
        }), 500

    try:

        return jsonify(
            json.loads(result.stdout)
        )

    except json.JSONDecodeError:

        return jsonify({
            "success": False,
            "message": "Invalid JSON returned by C++",
            "raw_output": result.stdout
        }), 500