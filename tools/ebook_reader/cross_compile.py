import os
import argparse

from invoke import Context

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def build_ebook_reader(
    c,
    recompile,
    benchmarks,
    debug,
    display,
):
    """Available displays:
    - it8951
    - x11
    - png

    """
    root_dir = os.path.join(SCRIPT_DIR, "..", "..")
    ereader_path = os.path.join(root_dir, "package", "ebook_reader")
    if not os.path.exists(ereader_path):
        return

    cross_tpl_path = os.path.join(SCRIPT_DIR, "..", "meson", "cross_compile.txt")

    with c.cd(ereader_path):
        build_path = os.path.join(root_dir, "build", os.path.basename(ereader_path))
        if recompile:
            c.run(f"rm -rf {build_path}")

        c.run(f"mkdir -p {build_path}")
        with open(cross_tpl_path, "r", encoding="utf-8") as f:
            cross_txt = f.read()
            cross_txt = cross_txt.replace("PLACEHOLDER", root_dir)

        cross_out_path = os.path.join(build_path, "cross-file.txt")
        print(f"{build_path=}")
        print(f"{cross_out_path=}")        
        with open(cross_out_path, "w", encoding="utf-8") as f:
            f.write(cross_txt)

        c.run(
            f"meson setup {build_path} -Ddisplay={display}"
            + (" --wipe " if recompile else " ")
            + (f" --cross-file {cross_out_path} ")
            + (" -Dbenchmarks=true " if benchmarks else " ")
            + (
                " -Dbuildtype=debug -Db_sanitize=address,undefined -Db_lundef=false "
                if debug
                else "  -Dbuildtype=release "
            )
        )

        c.run(
            f"rm -f compile_commands.json && ln -s {os.path.join(root_dir, 'build', 'compile_commands.json')} compile_commands.json"
        )

        c.run(f"meson compile -v -C {build_path}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--recompile",
        action="store_true",
        help="Recompile project",
    )

    parser.add_argument(
        "--benchmarks",
        action="store_true",
        help="Enable benchmarks",
    )

    parser.add_argument(
        "--debug",
        action="store_true",
        help="Enable debug mode",
    )

    
    parser.add_argument("--display", help="Display backend", default="it8951")

    args = parser.parse_args()

    c = Context()

    build_ebook_reader(
        c,
        recompile=args.recompile,
        benchmarks=args.benchmarks,
        debug=args.debug,
        display=args.display,
    )
