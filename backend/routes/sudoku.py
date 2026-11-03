import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

sudoku_bp = Blueprint("sudoku", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

SUDOKU_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit4"
    / "backtracking"
    / "sudoku.exe"
)


def extract_board(section):
    board = []

    for line in section.splitlines():
        numbers = re.findall(r"\d+", line)

        if len(numbers) == 9:
            board.append([int(number) for number in numbers])

    return board


def validate_sudoku(board):
    if len(board) != 9 or any(len(row) != 9 for row in board):
        return False

    expected = set(range(1, 10))

    # Validate rows.
    for row in board:
        if set(row) != expected:
            return False

    # Validate columns.
    for col in range(9):
        column = {board[row][col] for row in range(9)}

        if column != expected:
            return False

    # Validate 3x3 boxes.
    for start_row in range(0, 9, 3):
        for start_col in range(0, 9, 3):

            box = {
                board[row][col]
                for row in range(start_row, start_row + 3)
                for col in range(start_col, start_col + 3)
            }

            if box != expected:
                return False

    return True


@sudoku_bp.get("/api/v1/algorithms/sudoku")
def run_sudoku():

    try:
        if not SUDOKU_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Sudoku executable not found.",
                "expected_path": str(SUDOKU_EXE)
            }), 500

        result = subprocess.run(
            [str(SUDOKU_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=15
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Sudoku execution failed."
            }), 500

        output = result.stdout

        # Extract the initial board.
        if "Initial Sudoku Board:" not in output:
            raise ValueError("Initial Sudoku board not found.")

        initial_section = output.split(
            "Initial Sudoku Board:", 1
        )[1].split(
            "Solved Sudoku Board:", 1
        )[0]

        initial_board = extract_board(initial_section)

        if len(initial_board) != 9:
            raise ValueError(
                "Could not extract the complete initial Sudoku board."
            )

        # Extract the solved board.
        solution_found = "Solved Sudoku Board:" in output

        if solution_found:

            solved_section = output.split(
                "Solved Sudoku Board:", 1
            )[1].split(
                "Every row, column and 3x3 box", 1
            )[0]

            solved_board = extract_board(solved_section)

            if not validate_sudoku(solved_board):
                raise ValueError(
                    "The extracted Sudoku solution is invalid."
                )

        elif "No solution exists for this Sudoku." in output:

            solved_board = []

        else:
            raise ValueError("Sudoku solution status not found.")

        return jsonify({
            "success": True,
            "algorithm": "Sudoku Solver",
            "method": "Backtracking",
            "board_size": 9,
            "initial_board": initial_board,
            "solution_found": solution_found,
            "solved_board": solved_board,
            "time_complexity": "O(9^E) worst case",
            "space_complexity": "O(81 + E)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Sudoku execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500