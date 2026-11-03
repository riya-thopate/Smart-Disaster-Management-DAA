
import csv
from pathlib import Path

from flask import Blueprint, request, jsonify
from services.algorithm_runner import run_cpp_algorithm

emergency_algorithms_bp = Blueprint(
    "emergency_algorithms",
    __name__
)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"
EMERGENCIES_CSV = DATA_DIR / "emergencies.csv"
LOCATIONS_CSV = DATA_DIR / "locations.csv"


@emergency_algorithms_bp.get("/api/v1/emergencies/sort")
def sort_emergencies():
    order = request.args.get("order", "DESC").upper()

    if order not in {"ASC", "DESC"}:
        return jsonify({
            "success": False,
            "message": "order must be ASC or DESC"
        }), 400

    try:
        result = run_cpp_algorithm(
            "emergency_api.exe",
            ["SORT", order, str(EMERGENCIES_CSV)]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500


@emergency_algorithms_bp.get("/api/v1/emergencies/search")
def search_emergencies():
    query = request.args.get("q", "").strip()

    if not query:
        return jsonify({
            "success": False,
            "message": "Search query q is required"
        }), 400

    search_value = query
    matched_location = None

    try:
        # Translate a location name into its location ID.
        if LOCATIONS_CSV.exists():
            with open(
                LOCATIONS_CSV,
                "r",
                newline="",
                encoding="utf-8-sig"
            ) as file:
                reader = csv.DictReader(file)

                for row in reader:
                    name = row.get("name", "").strip()

                    if query.casefold() in name.casefold():
                        search_value = row.get(
                            "location_id", ""
                        ).strip()
                        matched_location = name
                        break

        # Run the actual C++ Linear Search.
        result = run_cpp_algorithm(
            "emergency_api.exe",
            ["SEARCH", search_value, str(EMERGENCIES_CSV)]
        )

        # Preserve what the user originally searched for.
        result["query"] = query

        if matched_location:
            result["matched_location"] = matched_location

        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
