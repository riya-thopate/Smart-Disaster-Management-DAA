import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

n_queens_bp = Blueprint("n_queens", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

N_QUEENS_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit4"
    / "backtracking"
    / "n_queens.exe"
)


@n_queens_bp.get("/api/v1/algorithms/n-queens")
def run_n_queens():

    try:
        if not N_QUEENS_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "N-Queens executable not found.",
                "expected_path": str(N_QUEENS_EXE)
            }), 500

        result = subprocess.run(
            [str(N_QUEENS_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "N-Queens execution failed."
            }), 500

        output = result.stdout

        # Extract board size.
        board_match = re.search(
            r"Emergency planning grid:\s*(\d+)\s*x\s*(\d+)",
            output
        )

        if not board_match:
            raise ValueError("Board dimensions not found.")

        rows = int(board_match.group(1))
        columns = int(board_match.group(2))

        if rows != columns:
            raise ValueError("N-Queens board must be square.")

        # Check whether a valid placement was found.
        if "Valid placement found:" not in output:
            return jsonify({
                "success": True,
                "algorithm": "N-Queens",
                "method": "Backtracking",
                "board_size": rows,
                "solution_found": False,
                "board": [],
                "queen_positions": [],
                "time_complexity": "O(N!)",
                "space_complexity": "O(N^2)",
                "output": output
            })

        # Extract the board section.
        board_section = output.split(
            "Valid placement found:", 1
        )[1].split(
            "Each response unit is placed", 1
        )[0]

        board = []

        for line in board_section.splitlines():

            tokens = line.strip().split()

            if (
                len(tokens) == columns
                and all(token in {"Q", "."} for token in tokens)
            ):
                board.append(tokens)

        if len(board) != rows:
            raise ValueError(
                "Could not extract the complete N-Queens board."
            )

        # Identify queen positions using 1-based indexing.
        queen_positions = []

        for row_index, board_row in enumerate(board):

            for column_index, cell in enumerate(board_row):

                if cell == "Q":
                    queen_positions.append({
                        "row": row_index + 1,
                        "column": column_index + 1
                    })

        if len(queen_positions) != rows:
            raise ValueError(
                "Number of queens does not match board size."
            )

        # Verify that all queens are safe.
        for i in range(len(queen_positions)):
            for j in range(i + 1, len(queen_positions)):

                first = queen_positions[i]
                second = queen_positions[j]

                same_column = (
                    first["column"] == second["column"]
                )

                same_diagonal = (
                    abs(first["row"] - second["row"])
                    ==
                    abs(first["column"] - second["column"])
                )

                if same_column or same_diagonal:
                    raise ValueError(
                        "Extracted solution violates queen placement rules."
                    )

        return jsonify({
            "success": True,
            "algorithm": "N-Queens Problem",
            "method": "Backtracking",
            "board_size": rows,
            "number_of_queens": len(queen_positions),
            "solution_found": True,
            "board": board,
            "queen_positions": queen_positions,
            "time_complexity": "O(N!)",
            "space_complexity": "O(N^2)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "N-Queens execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500