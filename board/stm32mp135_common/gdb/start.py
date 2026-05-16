import os
import sys
import tempfile

from invoke import Context

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

def run_gdb(c, phase="tf-a", runetime_attach=False):
    """
    Phase selects which firmware will be used for debugging.
    Available `phase` values are:
       - tf-a
       - optee-os
       - u-boot
       - linux

    Runetime attach decides whether to force reset before
    attaching debugger or attach it to CPU as it is.
    Setting `runetime_attach` to True means we won't do reset.
    """
    stage_phase_map = {
        "tf-a": 1,
        "optee-os": 2,
        "u-boot": 3,
        "linux": 4,
    }

    debug_phase = stage_phase_map.get(phase)
    if not debug_phase:
        raise ValueError(f"Wrong {phase=}")

    debug_mode = 0
    if runetime_attach:
        debug_mode = 1

    with open(os.path.join(SCRIPT_DIR, "init.gdb"), "r") as src:
        src_txt = src.read()

    src_txt = src_txt.replace(
        "set $debug_phase = 1", f"set $debug_phase = {debug_phase}", count=1
    )
    src_txt = src_txt.replace(
        "set $debug_mode = 0", f"set $debug_mode = {debug_mode}", count=1
    )

    with tempfile.NamedTemporaryFile(
        "w", prefix="init", suffix=".gdb", delete_on_close=False
    ) as dst:
        dst.write(src_txt)
        dst.close()

        with c.cd(SCRIPT_DIR):
            c.run(f"gdb-multiarch -x {str(dst.name)}", pty=True)

if __name__ == "__main__":
    c = Context()
    run_gdb(c, phase=sys.argv[1], runetime_attach=bool(int(sys.argv[2])))
    
