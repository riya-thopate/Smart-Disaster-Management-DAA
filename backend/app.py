from flask import Flask, jsonify
from flask_cors import CORS

from routes.data import data_bp
from routes.dashboard import dashboard_bp
from routes.emergency_algorithms import emergency_algorithms_bp
from routes.rescue_matching import rescue_matching_bp
from routes.max_flow import max_flow_bp
from routes.min_cut import min_cut_bp
from routes.fractional_knapsack import fractional_knapsack_bp
from routes.coin_change import coin_change_bp
from routes.mst import mst_bp
from routes.floyd_warshall import floyd_warshall_bp
from routes.tsp import tsp_bp
from routes.topological_sort import topological_sort_bp
from routes.scheduling import scheduling_bp
from routes.algorithm_lab import algorithm_lab_bp
from routes.simulation import simulation_bp
from routes.reports import reports_bp
from routes.optimal_bst import optimal_bst_bp
from routes.recursion import recursion_bp
from routes.merge_sort import merge_sort_bp
from routes.quick_sort import quick_sort_bp
from routes.closest_pair import closest_pair_bp
from routes.majority_element import majority_element_bp
from routes.order_statistics import order_statistics_bp
from routes.euclid import euclid_bp
from routes.karatsuba import karatsuba_bp
from routes.randomized import randomized_bp
from routes.fast_modular_exponentiation import fast_modular_bp
from routes.josephus import josephus_bp
from routes.tower_of_hanoi import tower_of_hanoi_bp
from routes.inversion_count import inversion_count_bp
from routes.fibonacci import fibonacci_bp
from routes.binomial import binomial_bp
from routes.matrix_chain import matrix_chain_bp
from routes.dag_shortest_path import dag_shortest_path_bp
from routes.lis import lis_bp
from routes.lcs import lcs_bp
from routes.independent_set import independent_set_bp
from routes.huffman import huffman_bp
from routes.graph_coloring import graph_coloring_bp
from routes.n_queens import n_queens_bp
from routes.hamiltonian_cycle import hamiltonian_cycle_bp
from routes.subset_sum import subset_sum_bp
from routes.sudoku import sudoku_bp
from routes.ford_fulkerson import ford_fulkerson_bp
from routes.sat import sat_bp
from routes.circuit_sat import circuit_sat_bp
from routes.three_sat import three_sat_bp
from routes.clique import clique_bp
from routes.vertex_cover import vertex_cover_bp
from routes.ilp import ilp_bp
try:
    from routes.evacuation import evacuation_bp
except ImportError:
    evacuation_bp = None

try:
    from routes.city_graph import city_graph_bp
except ImportError:
    city_graph_bp = None


# 1. Create the Flask application FIRST
app = Flask(__name__)
CORS(app)


# 2. Health endpoint
@app.get("/api/v1/health")
def health():
    return jsonify({
        "success": True,
        "status": "online",
        "message": "SmartDisaster backend is running",
        "version": "1.0.0"
    })


# 3. Register all blueprints AFTER creating the app
app.register_blueprint(data_bp)
app.register_blueprint(dashboard_bp)
app.register_blueprint(emergency_algorithms_bp)
app.register_blueprint(rescue_matching_bp)
app.register_blueprint(max_flow_bp)
app.register_blueprint(min_cut_bp)
app.register_blueprint(fractional_knapsack_bp)
if evacuation_bp is not None:
    app.register_blueprint(evacuation_bp)
app.register_blueprint(coin_change_bp)
app.register_blueprint(mst_bp)
app.register_blueprint(floyd_warshall_bp)
app.register_blueprint(tsp_bp)
if city_graph_bp is not None:
    app.register_blueprint(city_graph_bp)
app.register_blueprint(topological_sort_bp)
app.register_blueprint(scheduling_bp)
app.register_blueprint(algorithm_lab_bp)
app.register_blueprint(simulation_bp)
app.register_blueprint(reports_bp)
app.register_blueprint(optimal_bst_bp)
app.register_blueprint(recursion_bp)
app.register_blueprint(merge_sort_bp)
app.register_blueprint(quick_sort_bp)
app.register_blueprint(closest_pair_bp)
app.register_blueprint(majority_element_bp)
app.register_blueprint(order_statistics_bp)
app.register_blueprint(euclid_bp)
app.register_blueprint(karatsuba_bp)
app.register_blueprint(randomized_bp)
app.register_blueprint(fast_modular_bp)
app.register_blueprint(josephus_bp)
app.register_blueprint(tower_of_hanoi_bp)
app.register_blueprint(inversion_count_bp)
app.register_blueprint(fibonacci_bp)
app.register_blueprint(binomial_bp)
app.register_blueprint(matrix_chain_bp)
app.register_blueprint(dag_shortest_path_bp)
app.register_blueprint(lis_bp)
app.register_blueprint(lcs_bp)
app.register_blueprint(independent_set_bp)
app.register_blueprint(huffman_bp)
app.register_blueprint(graph_coloring_bp)
app.register_blueprint(n_queens_bp)
app.register_blueprint(hamiltonian_cycle_bp)
app.register_blueprint(subset_sum_bp)
app.register_blueprint(sudoku_bp)
app.register_blueprint(ford_fulkerson_bp)
app.register_blueprint(sat_bp)
app.register_blueprint(circuit_sat_bp)
app.register_blueprint(three_sat_bp)
app.register_blueprint(clique_bp)
app.register_blueprint(vertex_cover_bp)
app.register_blueprint(ilp_bp)
# 4. Start the server
if __name__ == "__main__":
    app.run(
        host="127.0.0.1",
        port=5000,
        debug=True
    )