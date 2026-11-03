import json
import subprocess
from pathlib import Path


BACKEND_ROOT = Path(__file__).resolve().parents[1]
CPP_BIN = BACKEND_ROOT / "cpp_bin"


def run_cpp_algorithm(
    executable_name: str,
    arguments: list[str] | None = None,
    timeout: int = 10,
) -> dict:

    arguments = arguments or []

    executable = CPP_BIN / executable_name

    if not executable.exists():
        raise FileNotFoundError(
            f"C++ executable not found: {executable}"
        )

    result = subprocess.run(
        [str(executable), *arguments],
        capture_output=True,
        text=True,
        timeout=timeout,
        check=False,
    )

    if result.returncode != 0:
        raise RuntimeError(
            result.stderr.strip()
            or "C++ algorithm execution failed"
        )

    try:
        return json.loads(result.stdout)
    except json.JSONDecodeError as error:
        raise RuntimeError(
            "C++ algorithm returned invalid JSON"
        ) from error