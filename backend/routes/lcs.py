import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

lcs_bp = Blueprint("lcs", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

LCS_EXE = (
    PROJECT_ROOT
    / "algorithms"
    / "unit3"
    / "lcs"
    / "lcs.exe"
)


def extract_words(section):
    return re.findall(r"\b[A-Z][A-Z0-9_]*\b", section)


@lcs_bp.get("/api/v1/algorithms/lcs")
def run_lcs():

    try:
        if not LCS_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "LCS executable not found."
            }), 500

        result = subprocess.run(
            [str(LCS_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "LCS execution failed."
            }), 500

        output = result.stdout

        # Extract Response Sequence 1.
        if "Response Sequence 1:" not in output:
            return jsonify({
                "success": False,
                "message": "Response Sequence 1 not found.",
                "output": output
            }), 500

        sequence1_section = output.split(
            "Response Sequence 1:", 1
        )[1].split(
            "Response Sequence 2:", 1
        )[0]

        sequence1 = extract_words(sequence1_section)

        # Extract Response Sequence 2.
        sequence2_section = output.split(
            "Response Sequence 2:", 1
        )[1].split(
            "Longest Common Subsequence:", 1
        )[0]

        sequence2 = extract_words(sequence2_section)

        # Extract the LCS.
        lcs_section = output.split(
            "Longest Common Subsequence:", 1
        )[1].split(
            "LCS Length", 1
        )[0]

        lcs_sequence = extract_words(lcs_section)

        # Extract LCS length.
        length_match = re.search(
            r"LCS Length\s*=\s*(\d+)",
            output
        )

        if not all([
            sequence1,
            sequence2,
            length_match
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract LCS results.",
                "output": output
            }), 500

        lcs_length = int(length_match.group(1))

        # Validate extracted sequence length.
        if len(lcs_sequence) != lcs_length:
            return jsonify({
                "success": False,
                "message": "Extracted LCS does not match reported length.",
                "output": output
            }), 500

        return jsonify({
            "success": True,
            "algorithm": "Longest Common Subsequence",
            "method": "Dynamic Programming",
            "sequence1": sequence1,
            "sequence2": sequence2,
            "lcs_sequence": lcs_sequence,
            "lcs_length": lcs_length,
            "time_complexity": "O(n * m)",
            "space_complexity": "O(n * m)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "LCS execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500