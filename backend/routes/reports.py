
import csv
import io
from pathlib import Path

from flask import Blueprint, jsonify, Response

reports_bp = Blueprint("reports", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"


def read_csv(filename):
    path = DATA_DIR / filename

    with open(path, "r", newline="", encoding="utf-8-sig") as file:
        return list(csv.DictReader(file))


@reports_bp.get("/api/v1/reports/summary")
def report_summary():
    try:
        emergencies = read_csv("emergencies.csv")
        teams = read_csv("rescue_teams.csv")
        shelters = read_csv("shelters.csv")
        resources = read_csv("resources.csv")
        roads = read_csv("roads.csv")

        active_emergencies = [
            e for e in emergencies
            if e["status"].strip().upper() == "ACTIVE"
        ]

        severity_counts = {}
        emergency_type_counts = {}

        for emergency in active_emergencies:
            severity = emergency["severity"].strip().upper()
            emergency_type = emergency["type"].strip().upper()

            severity_counts[severity] = (
                severity_counts.get(severity, 0) + 1
            )

            emergency_type_counts[emergency_type] = (
                emergency_type_counts.get(emergency_type, 0) + 1
            )

        people_affected = sum(
            int(e["affected_people"])
            for e in active_emergencies
        )

        available_teams = [
            team for team in teams
            if team["status"].strip().upper() == "AVAILABLE"
        ]

        total_capacity = sum(
            int(s["total_capacity"]) for s in shelters
        )

        occupied_capacity = sum(
            int(s["occupied"]) for s in shelters
        )

        available_shelter_capacity = sum(
            max(
                0,
                int(s["total_capacity"]) - int(s["occupied"])
            )
            for s in shelters
            if s["status"].strip().upper() == "OPEN"
        )

        resources_by_type = {}

        for resource in resources:
            resource_type = resource["type"].strip().upper()
            quantity = int(resource["quantity"])
            unit = resource["unit"].strip()

            if resource_type not in resources_by_type:
                resources_by_type[resource_type] = {
                    "quantity": 0,
                    "unit": unit,
                }

            if resources_by_type[resource_type]["unit"] != unit:
                return jsonify({
                    "success": False,
                    "message": (
                        f"Inconsistent measurement units for "
                        f"{resource_type}"
                    ),
                }), 500

            resources_by_type[resource_type]["quantity"] += quantity

        road_status_counts = {}

        for road in roads:
            status = road["status"].strip().upper()
            road_status_counts[status] = (
                road_status_counts.get(status, 0) + 1
            )

        report = {
            "success": True,
            "report_type": "Disaster Management Operational Summary",
            "data_source": "CSV",
            "emergencies": {
                "total": len(emergencies),
                "active": len(active_emergencies),
                "people_affected": people_affected,
                "by_severity": severity_counts,
                "by_type": emergency_type_counts,
            },
            "rescue_teams": {
                "total": len(teams),
                "available": len(available_teams),
            },
            "shelters": {
                "count": len(shelters),
                "total_capacity": total_capacity,
                "occupied": occupied_capacity,
                "available": available_shelter_capacity,
            },
            "resources": resources_by_type,
            "roads": {
                "total": len(roads),
                "by_status": road_status_counts,
            },
        }

        return jsonify(report)

    except (OSError, KeyError, ValueError) as error:
        return jsonify({
            "success": False,
            "message": str(error),
        }), 500


@reports_bp.get("/api/v1/reports/emergencies.csv")
def export_emergency_report():
    try:
        emergencies = read_csv("emergencies.csv")
        locations = read_csv("locations.csv")

        location_names = {
            location["location_id"].strip().upper():
            location["name"]
            for location in locations
        }

        output = io.StringIO()
        fields = [
            "emergency_id",
            "type",
            "severity",
            "location_id",
            "location_name",
            "affected_people",
            "required_resource",
            "status",
        ]

        writer = csv.DictWriter(output, fieldnames=fields)
        writer.writeheader()

        for emergency in emergencies:
            location_id = emergency["location_id"].strip().upper()

            writer.writerow({
                "emergency_id": emergency["emergency_id"],
                "type": emergency["type"],
                "severity": emergency["severity"],
                "location_id": location_id,
                "location_name": location_names.get(
                    location_id, "Unknown"
                ),
                "affected_people": emergency["affected_people"],
                "required_resource": emergency["required_resource"],
                "status": emergency["status"],
            })

        return Response(
            output.getvalue(),
            mimetype="text/csv",
            headers={
                "Content-Disposition":
                    'attachment; filename="emergency_report.csv"'
            },
        )

    except (OSError, KeyError, ValueError) as error:
        return jsonify({
            "success": False,
            "message": str(error),
        }), 500
