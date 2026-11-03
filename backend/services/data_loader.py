import csv
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"


def _read_csv(filename: str) -> list[dict]:
    path = DATA_DIR / filename

    if not path.exists():
        raise FileNotFoundError(
            f"Data file not found: {path}"
        )

    with path.open(
        "r",
        newline="",
        encoding="utf-8"
    ) as file:

        return list(csv.DictReader(file))


def get_locations() -> list[dict]:
    rows = _read_csv("locations.csv")

    return [
        {
            "id": row["location_id"],
            "name": row["name"],
            "type": row["type"],
            "x": float(row["x"]),
            "y": float(row["y"]),
        }
        for row in rows
    ]


def get_roads() -> list[dict]:
    rows = _read_csv("roads.csv")

    return [
        {
            "id": row["road_id"],
            "source": row["source"],
            "destination": row["destination"],
            "distance": int(row["distance"]),
            "time": int(row["time"]),
            "capacity": int(row["capacity"]),
            "status": row["status"],
        }
        for row in rows
    ]


def get_shelters() -> list[dict]:
    rows = _read_csv("shelters.csv")

    return [
        {
            "id": row["shelter_id"],
            "location_id": row["location_id"],
            "name": row["name"],
            "total_capacity": int(row["total_capacity"]),
            "occupied": int(row["occupied"]),
            "available": (
                int(row["total_capacity"])
                - int(row["occupied"])
            ),
            "status": row["status"],
        }
        for row in rows
    ]


def get_emergencies() -> list[dict]:
    rows = _read_csv("emergencies.csv")

    return [
        {
            "id": row["emergency_id"],
            "location_id": row["location_id"],
            "type": row["type"],
            "severity": row["severity"],
            "affected_people": int(
                row["affected_people"]
            ),
            "required_resource": row[
                "required_resource"
            ],
            "status": row["status"],
        }
        for row in rows
    ]


def get_rescue_teams() -> list[dict]:
    rows = _read_csv("rescue_teams.csv")

    return [
        {
            "id": row["team_id"],
            "name": row["name"],
            "type": row["type"],
            "location_id": row["location_id"],
            "capacity": int(row["capacity"]),
            "status": row["status"],
        }
        for row in rows
    ]


def get_resources() -> list[dict]:
    rows = _read_csv("resources.csv")

    return [
        {
            "id": row["resource_id"],
            "type": row["type"],
            "location_id": row["location_id"],
            "quantity": int(row["quantity"]),
            "unit": row["unit"],
            "priority": int(row["priority"]),
        }
        for row in rows
    ]