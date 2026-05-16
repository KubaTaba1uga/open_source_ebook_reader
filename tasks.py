"""
SPDX-License-Identifier: MIT

Copyright 2026 Jakub Buczynski <KubaTaba1uga>
"""

import os

from invoke import task

ROOT_PATH = os.path.dirname(os.path.abspath(__file__))
BUILD_PATH = os.path.join(ROOT_PATH, "build")
DOCS_PATH = os.path.join(ROOT_PATH, "docs")
PACKAGES_PATH = os.path.join(ROOT_PATH, "package")

C_FORMATER = "clang-format-19"
C_LINTER = "clang-tidy-19"

os.environ["PATH"] = f"{os.path.join(ROOT_PATH, '.venv', 'bin')}:{os.environ['PATH']}"
os.chdir(ROOT_PATH)


@task
def deps_install(c):
    _pr_info(f"Installing dependencies...")

    try:
        c.run(
            "sudo apt-get install -y doxygen virtualenv git python3"
            f" {C_FORMATER} {C_LINTER} "
        )

        c.run("virtualenv .venv")
        c.run(
            "pip install invoke sphinx==8.2.3 breathe==4.36.0 sphinx_rtd_theme==3.0.2 sphinx-autobuild==2025.08.25 myst-parser==5.0.0"
        )

    except Exception:
        _pr_error("Installing failed")
        raise

    _pr_info(f"Installing dependencies completed")


@task
def docs_build(c):
    _pr_info("Building docs...")

    docs_path = os.path.join(BUILD_PATH, "docs", "html")

    try:
        c.run("doxygen docs/Doxyfile")
        with c.cd(DOCS_PATH):
            c.run(f"sphinx-build -b html . {docs_path}")
    except Exception:
        _pr_error(f"Building docs failed")
        raise

    _pr_info("Building docs completed")


@task
def docs_serve(c, port=8000):
    _pr_info("Serving docs...")

    docs_build(c)

    c.run(
        " ".join(
            [
                f"sphinx-autobuild",
                f"--port {port}",
                f"docs build/docs/html",
            ]
        ),
        pty=True,
    )

    _pr_info("Serving docs completed")


###############################################
#                Private API                  #
###############################################
def _pr_info(message: str):
    print(f"\033[94m[INFO] {message}\033[0m")


def _pr_warn(message: str):
    print(f"\033[93m[WARN] {message}\033[0m")


def _pr_debug(message: str):
    print(f"\033[96m[DEBUG] {message}\033[0m")


def _pr_error(message: str):
    print(f"\033[91m[ERROR] {message}\033[0m")


