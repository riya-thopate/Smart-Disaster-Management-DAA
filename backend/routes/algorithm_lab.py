
import math
from pathlib import Path

from flask import Blueprint, request, jsonify
from services.algorithm_runner import run_cpp_algorithm

algorithm_lab_bp = Blueprint("algorithm_lab", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = PROJECT_ROOT / "data"
CPP_BIN = PROJECT_ROOT / "backend" / "cpp_bin"

CATALOG = [
    {
        "id": "bfs",
        "name": "Breadth-First Search",
        "unit": 1,
        "executable": "bfs_api.exe",
        "time_complexity": "O(V + E)",
        "space_complexity": "O(V)",
    },
    {
        "id": "dfs",
        "name": "Depth-First Search",
        "unit": 1,
        "executable": "dfs_api.exe",
        "time_complexity": "O(V + E)",
        "space_complexity": "O(V)",
    },
    {
        "id": "dijkstra",
        "name": "Dijkstra's Shortest Path",
        "unit": 4,
        "executable": "dijkstra_api.exe",
        "time_complexity": "O((V + E) log V)",
        "space_complexity": "O(V + E)",
    },
    {
        "id": "emergency_sort",
        "name": "Emergency Merge Sort",
        "unit": 1,
        "executable": "emergency_api.exe",
        "time_complexity": "O(n log n)",
        "space_complexity": "O(n)",
    },
    {
        "id": "emergency_search",
        "name": "Emergency Linear Search",
        "unit": 1,
        "executable": "emergency_api.exe",
        "time_complexity": "O(n)",
        "space_complexity": "O(n)",
    },
    {
        "id": "rescue_matching",
        "name": "Bipartite Rescue Matching",
        "unit": 5,
        "executable": "rescue_matching_api.exe",
        "time_complexity": "O(V * E)",
        "space_complexity": "O(V + E)",
    },
    {
        "id": "max_flow",
        "name": "Maximum Flow",
        "unit": 5,
        "executable": "max_flow_api.exe",
        "time_complexity": "O(V * E^2)",
        "space_complexity": "O(V + E)",
    },
    {
        "id": "min_cut",
        "name": "Minimum Cut",
        "unit": 5,
        "executable": "min_cut_api.exe",
        "time_complexity": "O(V * E^2)",
        "space_complexity": "O(V + E)",
    },
    {
        "id": "fractional_knapsack",
        "name": "Fractional Knapsack",
        "unit": 4,
        "executable": "fractional_knapsack_api.exe",
        "time_complexity": "O(n log n)",
        "space_complexity": "O(n)",
    },
    {
        "id": "coin_change",
        "name": "Coin Change",
        "unit": 3,
        "executable": "coin_change_api.exe",
        "time_complexity": "O(amount * n)",
        "space_complexity": "O(amount)",
    },
    {
        "id": "mst",
        "name": "Kruskal's Minimum Spanning Tree",
        "unit": 4,
        "executable": "mst_api.exe",
        "time_complexity": "O(E log E)",
        "space_complexity": "O(V + E)",
    },
    {
        "id": "floyd_warshall",
        "name": "Floyd-Warshall",
        "unit": 3,
        "executable": "floyd_warshall_api.exe",
        "time_complexity": "O(V^3)",
        "space_complexity": "O(V^2)",
    },
    {
        "id": "tsp",
        "name": "Travelling Salesman Problem",
        "unit": 3,
        "executable": "tsp_api.exe",
        "time_complexity": "O(V^3 + k^2 * 2^k)",
        "space_complexity": "O(V^2 + k * 2^k)",
    },
    {
        "id": "topological_sort",
        "name": "Topological Sort",
        "unit": 1,
        "executable": "topological_sort_api.exe",
        "time_complexity": "O(V + E)",
        "space_complexity": "O(V + E)",
    },
    {
        "id": "scheduling",
        "name": "Job Sequencing with Deadlines",
        "unit": 4,
        "executable": "scheduling_api.exe",
        "time_complexity": "O(n log n + nD)",
        "space_complexity": "O(n + D)",
    },
]


@algorithm_lab_bp.get("/api/v1/algorithms/lab")
def list_algorithms():
    algorithms = []

    for item in CATALOG:
        algorithms.append({
            "id": item["id"],
            "name": item["name"],
            "unit": item["unit"],
            "time_complexity": item["time_complexity"],
            "space_complexity": item["space_complexity"],
            "available": (
                CPP_BIN / item["executable"]
            ).is_file(),
        })

    return jsonify({
        "success": True,
        "count": len(algorithms),
        "algorithms": algorithms,
    })


@algorithm_lab_bp.post("/api/v1/algorithms/lab/run")
def run_lab_algorithm():
    payload = request.get_json(silent=True)

    if not isinstance(payload, dict):
        return jsonify({
            "success": False,
            "message": "Send a JSON object containing an algorithm ID",
        }), 400

    algorithm = str(
        payload.get("algorithm", "")
    ).strip().lower()

    spec = next(
        (item for item in CATALOG if item["id"] == algorithm),
        None,
    )

    if spec is None:
        return jsonify({
            "success": False,
            "message": "Unsupported algorithm ID",
            "supported_algorithms": [
                item["id"] for item in CATALOG
            ],
        }), 400

    def location_id(field, default):
        value = str(
            payload.get(field, default)
        ).strip().upper()

        if not value:
            raise ValueError(f"{field} is required")

        return value

    try:
        if algorithm in {"bfs", "dfs"}:
            args = [location_id("source", "L10")]

        elif algorithm == "dijkstra":
            source = location_id("source", "L10")
            destination = location_id("destination", "L7")

            if source == destination:
                raise ValueError(
                    "Source and destination must differ"
                )

            args = [source, destination]

        elif algorithm == "emergency_sort":
            order = str(
                payload.get("order", "DESC")
            ).strip().upper()

            if order not in {"ASC", "DESC"}:
                raise ValueError("order must be ASC or DESC")

            args = [
                "SORT",
                order,
                str(DATA_DIR / "emergencies.csv"),
            ]

        elif algorithm == "emergency_search":
            query = str(
                payload.get("query", "")
            ).strip()

            if not query:
                raise ValueError("query is required")

            args = [
                "SEARCH",
                query,
                str(DATA_DIR / "emergencies.csv"),
            ]

        elif algorithm == "rescue_matching":
            args = [
                str(DATA_DIR / "emergencies.csv"),
                str(DATA_DIR / "rescue_teams.csv"),
            ]

        elif algorithm in {"max_flow", "min_cut"}:
            source = location_id("source", "L10")
            destination = location_id("destination", "L7")

            if source == destination:
                raise ValueError(
                    "Source and destination must differ"
                )

            args = [
                str(DATA_DIR / "roads.csv"),
                source,
                destination,
            ]

        elif algorithm == "fractional_knapsack":
            resource_type = str(
                payload.get("resource_type", "WATER")
            ).strip().upper()

            if resource_type not in {
                "WATER", "FOOD", "MEDICINE", "BLANKET"
            }:
                raise ValueError("Unsupported resource_type")

            raw_capacity = payload.get("capacity", 1200)

            if isinstance(raw_capacity, bool):
                raise ValueError("capacity must be a number")

            capacity = float(raw_capacity)

            if not math.isfinite(capacity) or capacity <= 0:
                raise ValueError(
                    "capacity must be a positive finite number"
                )

            args = [
                str(DATA_DIR / "resources.csv"),
                resource_type,
                str(capacity),
            ]

        elif algorithm == "coin_change":
            amount = payload.get("amount", 63)

            if isinstance(amount, bool) or not isinstance(amount, int):
                raise ValueError("amount must be an integer")

            if amount < 0 or amount > 1_000_000:
                raise ValueError(
                    "amount must be between 0 and 1000000"
                )

            denominations = payload.get(
                "denominations", "1,5,10,25"
            )

            if isinstance(denominations, list):
                if not denominations or any(
                    isinstance(x, bool)
                    or not isinstance(x, int)
                    or x <= 0
                    for x in denominations
                ):
                    raise ValueError(
                        "Denominations must be positive integers"
                    )

                denominations = ",".join(
                    str(x) for x in denominations
                )

            elif (
                not isinstance(denominations, str)
                or not denominations.strip()
            ):
                raise ValueError(
                    "denominations must be a comma-separated string "
                    "or list of positive integers"
                )

            args = [str(amount), denominations]

        elif algorithm == "mst":
            args = [str(DATA_DIR / "roads.csv")]

        elif algorithm == "floyd_warshall":
            source = location_id("source", "L10")
            destination = location_id("destination", "L7")

            if source == destination:
                raise ValueError(
                    "Source and destination must differ"
                )

            args = [
                str(DATA_DIR / "roads.csv"),
                source,
                destination,
            ]

        elif algorithm == "tsp":
            start = location_id("start", "L8")

            args = [
                str(DATA_DIR / "roads.csv"),
                str(DATA_DIR / "emergencies.csv"),
                str(DATA_DIR / "locations.csv"),
                start,
            ]

        elif algorithm == "topological_sort":
            args = [str(DATA_DIR / "tasks.csv")]

        elif algorithm == "scheduling":
            args = [str(DATA_DIR / "scheduling_jobs.csv")]

        else:
            raise ValueError(
                "No runner configured for this algorithm"
            )

        result = run_cpp_algorithm(
            spec["executable"],
            args,
            timeout=30,
        )

        return jsonify(result)

    except (ValueError, TypeError) as error:
        return jsonify({
            "success": False,
            "message": str(error),
        }), 400

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error),
        }), 500
