"""Build and run the real probe against mocked IL2CPP on any host with a C compiler."""
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix="songprobe-") as tmp:
    exe = Path(tmp) / ("probe.exe" if os.name == "nt" else "probe")
    subprocess.run([
        os.environ.get("CC", "gcc"), "-std=c11", "-Wall", "-Wextra", "-Werror",
        "-Iruntime", "-Iruntime/guest", "tests/t_songprobe.c",
        "runtime/guest/kl_songprobe.c", "-o", str(exe),
    ], cwd=ROOT, check=True)
    cases = {
        "success": "BRIDGE_OK",
        "exception": "STOP Debug.Log threw",
        "disabled": None,
        "other-target": None,
        "no-image": "STOP managed domain/classes",
        "missing-export": "STOP missing export il2cpp_string_new",
        "no-assemblies": "STOP managed domain/classes",
        "no-model": "STOP managed domain/classes",
        "no-log": "STOP Debug.Log/1",
    }
    for scenario, expected in cases.items():
        p = subprocess.run([str(exe), scenario], capture_output=True, text=True, check=True)
        assert p.stdout.strip() == "PASS", p
        if expected:
            assert expected in p.stderr, p.stderr
        else:
            assert not p.stderr, p.stderr
        if scenario != "success":
            assert "BRIDGE_OK" not in p.stderr, p.stderr
        else:
            assert p.stderr.count("BRIDGE_OK") == 1, p.stderr
            assert "DONE capability inventory only" in p.stderr, p.stderr
        print(f"PASS {scenario}")
