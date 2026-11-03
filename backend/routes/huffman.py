import re
import subprocess
from pathlib import Path

from flask import Blueprint, jsonify

huffman_bp = Blueprint("huffman", __name__)

PROJECT_ROOT = Path(__file__).resolve().parents[2]

HUFFMAN_CANDIDATES = [
    PROJECT_ROOT / "algorithms" / "unit4" / "huffman" / "huffman.exe",
    PROJECT_ROOT / "algorithms" / "unit4" / "huffman.exe",
]

HUFFMAN_EXE = next(
    (path for path in HUFFMAN_CANDIDATES if path.is_file()),
    HUFFMAN_CANDIDATES[0]
)


def extract_section(output, start_marker, end_marker):
    start = output.find(start_marker)

    if start == -1:
        raise ValueError(f"Section not found: {start_marker}")

    start += len(start_marker)
    end = output.find(end_marker, start)

    if end == -1:
        raise ValueError(f"Section ending not found: {end_marker}")

    return output[start:end]


@huffman_bp.get("/api/v1/algorithms/huffman")
def run_huffman():

    try:
        if not HUFFMAN_EXE.is_file():
            return jsonify({
                "success": False,
                "message": "Huffman executable not found.",
                "checked_paths": [
                    str(path) for path in HUFFMAN_CANDIDATES
                ]
            }), 500

        result = subprocess.run(
            [str(HUFFMAN_EXE)],
            cwd=str(PROJECT_ROOT),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode != 0:
            return jsonify({
                "success": False,
                "message": result.stderr or "Huffman execution failed."
            }), 500

        output = result.stdout

        report_match = re.search(
            r"Disaster report text:\s*\n([A-Z]+)",
            output
        )

        original_match = re.search(
            r"Original size\s*=\s*(\d+)\s*bits",
            output
        )

        compressed_match = re.search(
            r"Compressed size\s*=\s*(\d+)\s*bits",
            output
        )

        encoded_match = re.search(
            r"Encoded report\s*=\s*([01]+)",
            output
        )

        if not all([
            report_match,
            original_match,
            compressed_match,
            encoded_match
        ]):
            return jsonify({
                "success": False,
                "message": "Could not extract Huffman results.",
                "output": output
            }), 500

        report = report_match.group(1)

        # Extract character frequencies.
        frequency_section = extract_section(
            output,
            "Character Frequencies:",
            "----------------------------------------"
        )

        frequency_matches = re.findall(
            r"^\s*([A-Z])\s*->\s*(\d+)\s*$",
            frequency_section,
            re.MULTILINE
        )

        character_frequencies = {
            character: int(frequency)
            for character, frequency in frequency_matches
        }

        # Extract Huffman codes.
        code_section = extract_section(
            output,
            "Huffman Codes:",
            "----------------------------------------"
        )

        code_matches = re.findall(
            r"^\s*([A-Z])\s*->\s*([01]+)\s*$",
            code_section,
            re.MULTILINE
        )

        huffman_codes = {
            character: code
            for character, code in code_matches
        }

        original_bits = int(original_match.group(1))
        compressed_bits = int(compressed_match.group(1))
        encoded_report = encoded_match.group(1)

        if not character_frequencies or not huffman_codes:
            return jsonify({
                "success": False,
                "message": "Character frequencies or Huffman codes are missing.",
                "output": output
            }), 500

        if set(character_frequencies) != set(huffman_codes):
            return jsonify({
                "success": False,
                "message": "Frequency and code character sets do not match.",
                "output": output
            }), 500

        if len(encoded_report) != compressed_bits:
            return jsonify({
                "success": False,
                "message": "Encoded length does not match compressed size."
            }), 500

        if sum(character_frequencies.values()) != len(report):
            return jsonify({
                "success": False,
                "message": "Character frequencies do not match report length."
            }), 500

        savings = original_bits - compressed_bits

        return jsonify({
            "success": True,
            "algorithm": "Huffman Coding",
            "method": "Greedy Algorithm",
            "report_text": report,
            "character_count": len(report),
            "unique_characters": len(character_frequencies),
            "character_frequencies": [
                {
                    "character": character,
                    "frequency": frequency
                }
                for character, frequency
                in sorted(character_frequencies.items())
            ],
            "huffman_codes": [
                {
                    "character": character,
                    "code": code
                }
                for character, code
                in sorted(huffman_codes.items())
            ],
            "original_bits": original_bits,
            "compressed_bits": compressed_bits,
            "encoded_report": encoded_report,
            "bits_saved": savings,
            "space_saving_percentage": round(
                savings / original_bits * 100, 2
            ),
            "time_complexity": "O(n + k log k)",
            "space_complexity": "O(n + k)",
            "output": output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success": False,
            "message": "Huffman execution timed out."
        }), 500

    except Exception as error:
        return jsonify({
            "success": False,
            "message": str(error)
        }), 500