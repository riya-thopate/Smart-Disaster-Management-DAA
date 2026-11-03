
import csv
import heapq
from pathlib import Path

from flask import Blueprint, request, jsonify

simulation_bp = Blueprint("simulation", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"


def read_csv(filename):
    path = DATA_DIR / filename

    with open(path, "r", newline="", encoding="utf-8-sig") as file:
        return list(csv.DictReader(file))


def shortest_route(source, destination, roads, locations, blocked_roads):
    graph = {location_id: [] for location_id in locations}

    for road in roads:
        if road["status"].strip().upper() != "OPEN":
            continue

        if road["road_id"].strip().upper() in blocked_roads:
            continue

        u = road["source"].strip().upper()
        v = road["destination"].strip().upper()
        distance = int(road["distance"])
        travel_time = int(road["time"])
        road_id = road["road_id"].strip().upper()

        # The project's roads are treated as bidirectional.
        graph[u].append((v, distance, travel_time, road_id))
        graph[v].append((u, distance, travel_time, road_id))

    distances = {source: 0}
    previous = {}
    queue = [(0, source)]

    # Dijkstra: minimize total distance.
    while queue:
        current_distance, u = heapq.heappop(queue)

        if current_distance != distances.get(u):
            continue

        if u == destination:
            break

        for v, distance, travel_time, road_id in graph.get(u, []):
            candidate = current_distance + distance

            if candidate < distances.get(v, float("inf")):
                distances[v] = candidate
                previous[v] = (
                    u,
                    distance,
                    travel_time,
                    road_id,
                )
                heapq.heappush(queue, (candidate, v))

    if destination not in distances:
        return {
            "reachable": False,
            "distance": None,
            "estimated_time": None,
            "path": [],
            "path_names": [],
            "roads": [],
        }

    path = [destination]
    selected_roads = []
    current = destination
    total_time = 0

    while current != source:
        if current not in previous:
            raise RuntimeError("Could not reconstruct the selected route")

        parent, distance, travel_time, road_id = previous[current]

        selected_roads.append({
            "road_id": road_id,
            "from": parent,
            "to": current,
            "distance": distance,
            "time": travel_time,
        })

        total_time += travel_time
        current = parent
        path.append(current)

    path.reverse()
    selected_roads.reverse()

    return {
        "reachable": True,
        "distance": distances[destination],
        "estimated_time": total_time,
        "path": path,
        "path_names": [
            locations[node]["name"] for node in path
        ],
        "roads": selected_roads,
    }


@simulation_bp.post("/api/v1/simulation/run")
def run_simulation():
    payload = request.get_json(silent=True)

    if not isinstance(payload, dict):
        return jsonify({
            "success": False,
            "message": "A JSON object is required",
        }), 400

    source = str(payload.get("source", "L10")).strip().upper()
    destination = str(payload.get("destination", "L7")).strip().upper()

    scenario_name = str(
        payload.get("scenario_name", "Road closure simulation")
    ).strip()

    raw_blocked = payload.get("blocked_roads", [])

    if not source or not destination:
        return jsonify({
            "success": False,
            "message": "Source and destination are required",
        }), 400

    if source == destination:
        return jsonify({
            "success": False,
            "message": "Source and destination must differ",
        }), 400

    if not isinstance(raw_blocked, list):
        return jsonify({
            "success": False,
            "message": "blocked_roads must be a list of road IDs",
        }), 400

    if any(not isinstance(item, str) or not item.strip()
           for item in raw_blocked):
        return jsonify({
            "success": False,
            "message": "Every blocked road must be a non-empty string",
        }), 400

    blocked_roads = {
        item.strip().upper() for item in raw_blocked
    }

    try:
        roads = read_csv("roads.csv")
        location_rows = read_csv("locations.csv")
        emergencies = read_csv("emergencies.csv")
        shelters = read_csv("shelters.csv")

        locations = {
            row["location_id"].strip().upper(): row
            for row in location_rows
        }

        if source not in locations or destination not in locations:
            return jsonify({
                "success": False,
                "message": "Unknown source or destination location ID",
            }), 400

        known_road_ids = {
            row["road_id"].strip().upper() for row in roads
        }

        unknown_roads = sorted(blocked_roads - known_road_ids)

        if unknown_roads:
            return jsonify({
                "success": False,
                "message": "Unknown road ID(s)",
                "unknown_roads": unknown_roads,
            }), 400

        baseline = shortest_route(
            source, destination, roads, locations, set()
        )

        simulated = shortest_route(
            source, destination, roads, locations, blocked_roads
        )

        if baseline["reachable"] and simulated["reachable"]:
            distance_change = (
                simulated["distance"] - baseline["distance"]
            )
            time_change = (
                simulated["estimated_time"]
                - baseline["estimated_time"]
            )
        else:
            distance_change = None
            time_change = None

        active_emergencies = [
            row for row in emergencies
            if row["status"].strip().upper() == "ACTIVE"
        ]

        people_affected = sum(
            int(row["affected_people"])
            for row in active_emergencies
        )

        available_shelter_capacity = sum(
            max(
                0,
                int(row["total_capacity"]) - int(row["occupied"])
            )
            for row in shelters
            if row["status"].strip().upper() == "OPEN"
        )

        return jsonify({
            "success": True,
            "simulation": "Road-Closure Simulation",
            "scenario_name": scenario_name,
            "source": source,
            "destination": destination,
            "blocked_roads": sorted(blocked_roads),
            "baseline": baseline,
            "simulated": simulated,
            "impact": {
                "route_available": simulated["reachable"],
                "distance_change": distance_change,
                "estimated_time_change": time_change,
            },
            "context_metrics": {
                "active_emergencies": len(active_emergencies),
                "people_affected": people_affected,
                "available_shelter_capacity": available_shelter_capacity,
            },
            "note": (
                "This is a CSV-based simulation. It does not change "
                "the underlying CSV files or live system data."
            ),
        })

    except (OSError, KeyError, ValueError) as error:
        return jsonify({
            "success": False,
            "message": str(error),
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error),
        }), 500
