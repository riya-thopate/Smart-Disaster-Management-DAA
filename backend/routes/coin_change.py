
from flask import Blueprint, request, jsonify
from services.algorithm_runner import run_cpp_algorithm

coin_change_bp = Blueprint("coin_change", __name__)

MAX_AMOUNT = 1_000_000


@coin_change_bp.get("/api/v1/algorithms/coin-change")
def calculate_coin_change():
    amount_text = request.args.get("amount", "63").strip()
    denominations = request.args.get(
        "denominations", "1,5,10,25"
    ).strip()

    try:
        amount = int(amount_text)

        if amount < 0 or amount > MAX_AMOUNT:
            return jsonify({
                "success": False,
                "message": (
                    f"amount must be between 0 and {MAX_AMOUNT}"
                )
            }), 400

    except ValueError:
        return jsonify({
            "success": False,
            "message": "amount must be an integer"
        }), 400

    if not denominations:
        return jsonify({
            "success": False,
            "message": "denominations are required"
        }), 400

    try:
        result = run_cpp_algorithm(
            "coin_change_api.exe",
            [str(amount), denominations]
        )
        return jsonify(result)

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500
