from flask import Blueprint, jsonify

from services.data_loader import (
    get_emergencies,
    get_rescue_teams,
    get_shelters,
    get_resources,
)


dashboard_bp = Blueprint(
    "dashboard",
    __name__
)


@dashboard_bp.get("/api/v1/dashboard")
def dashboard():

    emergencies = get_emergencies()
    teams = get_rescue_teams()
    shelters = get_shelters()
    resources = get_resources()

    active_emergencies = [
        item
        for item in emergencies
        if item["status"] == "ACTIVE"
    ]

    available_teams = [
        item
        for item in teams
        if item["status"] == "AVAILABLE"
    ]

    people_affected = sum(
        item["affected_people"]
        for item in active_emergencies
    )

    total_capacity = sum(
        item["total_capacity"]
        for item in shelters
    )

    occupied_capacity = sum(
        item["occupied"]
        for item in shelters
    )

    resource_summary = {}

    for resource in resources:

        resource_type = resource["type"]

        resource_summary.setdefault(
            resource_type,
            0
        )

        resource_summary[resource_type] += (
            resource["quantity"]
        )

    return jsonify({
        "success": True,

        "emergencies": {
            "active": len(active_emergencies),
            "total": len(emergencies),
            "people_affected": people_affected,
        },

        "rescue_teams": {
            "total": len(teams),
            "available": len(available_teams),
        },

        "shelters": {
            "total_capacity": total_capacity,
            "occupied": occupied_capacity,
            "available": (
                total_capacity
                - occupied_capacity
            ),
        },

        "resources": resource_summary,

        "system": {
            "data_source": "CSV",
            "mode": "backend",
        },
    })