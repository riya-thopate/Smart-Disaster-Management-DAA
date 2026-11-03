from flask import Blueprint, jsonify

from services.data_loader import (
    get_locations,
    get_roads,
    get_shelters,
    get_emergencies,
    get_rescue_teams,
    get_resources,
)


data_bp = Blueprint(
    "data",
    __name__
)


@data_bp.get("/api/v1/locations")
def locations():

    return jsonify({
        "success": True,
        "count": len(get_locations()),
        "locations": get_locations(),
    })


@data_bp.get("/api/v1/roads")
def roads():

    return jsonify({
        "success": True,
        "count": len(get_roads()),
        "roads": get_roads(),
    })


@data_bp.get("/api/v1/shelters")
def shelters():

    return jsonify({
        "success": True,
        "count": len(get_shelters()),
        "shelters": get_shelters(),
    })


@data_bp.get("/api/v1/emergencies")
def emergencies():

    data = get_emergencies()

    return jsonify({
        "success": True,
        "count": len(data),
        "emergencies": data,
    })


@data_bp.get("/api/v1/rescue-teams")
def rescue_teams():

    data = get_rescue_teams()

    return jsonify({
        "success": True,
        "count": len(data),
        "rescue_teams": data,
    })


@data_bp.get("/api/v1/resources")
def resources():

    data = get_resources()

    return jsonify({
        "success": True,
        "count": len(data),
        "resources": data,
    })