"""
SPDX-License-Identifier: MIT

Copyright 2026 Jakub Buczynski <KubaTaba1uga>
"""

import os

from invoke import task

ROOT_PATH = os.path.dirname(os.path.abspath(__file__))
BUILD_PATH = os.path.join(ROOT_PATH, "build")
CONFIG_PATH = os.path.join(ROOT_PATH, "configs")
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
            "sudo apt-get install -y " +
            " ".join(
            ["which sed make binutils build-essential diffutils",
             "gcc g++ bash patch gzip bzip2 perl tar cpio",
             "unzip rsync file bc findutils gawk curl",
             "libncurses5-dev python3 libpoppler-glib-dev poppler-utils",            
             C_FORMATER, C_LINTER]
        ))

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


@task
def image_build(c, config="stm32mp135d_odyssey_defconfig"):
    _pr_info(f"Building image...")

    config_path = os.path.join(ROOT_PATH, "configs", config)
    config_dict = _parse_config(config_path)

    repos = _find_repos_in_br_config(config_dict)

    if "debug" in config:
        c.run("mkdir -p third_party")
        with c.cd("third_party"):
            for repo, rdata in repos.items():
                if os.path.exists(os.path.join(ROOT_PATH, "third_party", repo)):
                    continue

                c.run(f"git clone {rdata['url']} {repo}")
                with c.cd(repo):
                    c.run(f"git checkout {rdata['tag']}")

                    patches_dir = os.path.join(
                        ROOT_PATH,
                        config_dict["BR2_GLOBAL_PATCH_DIR"].replace(
                            "$(BR2_EXTERNAL_EBK_READER_PATH)", ""
                        ),
                        repo,
                    )
                    if os.path.exists(patches_dir):
                        c.run(
                            f"find {patches_dir} -type f -name '*.patch' -exec git apply {{}} \\;"
                        )

    if config:
        image_configure(c, config)

    with c.cd("build/buildroot"):
        c.run("make -j24")

    _pr_info(f"Building image completed")


@task
def image_configure(c, config="stm32mp135d_odyssey_defconfig"):
    _pr_info(f"Configuring image...")

    with c.cd("third_party/buildroot"):
        flags = [
            "O=../../build/buildroot",
            "BR2_EXTERNAL=../../",
            config,
        ]

        c.run(f"make " + " ".join(flags))

    _pr_info(f"Configuring image completed")


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


def _parse_config(config_path: str) -> dict:
    result = {}

    with open(config_path, "r") as config_fp:
        config = config_fp.readlines()

    for line in config:
        if line.startswith("#") or not "=" in line:
            continue
        key, value = line.split("=", maxsplit=1)

        for i, char in enumerate(value):
            if char == "#":
                value = value[:i]
                break

        result[key] = value.strip()

    return result


def _find_repos_in_br_config(config_dict: dict):
    repo_version_map = {
        "tf-a": {
            "url": "BR2_TARGET_ARM_TRUSTED_FIRMWARE_CUSTOM_REPO_URL",
            "version": "BR2_TARGET_ARM_TRUSTED_FIRMWARE_CUSTOM_REPO_VERSION",
        },
        "optee-os": {
            "url": "BR2_TARGET_OPTEE_OS_CUSTOM_REPO_URL",
            "version": "BR2_TARGET_OPTEE_OS_CUSTOM_REPO_VERSION",
        },
        "u-boot": {
            "url": "BR2_TARGET_UBOOT_CUSTOM_REPO_URL",
            "version": "BR2_TARGET_UBOOT_CUSTOM_REPO_VERSION",
        },
        "linux": {
            "url": "BR2_LINUX_KERNEL_CUSTOM_REPO_URL",
            "version": "BR2_LINUX_KERNEL_CUSTOM_REPO_VERSION",
        },
    }
    results = {
        "buildroot": {
            "version": "st/2024.02.9",
            "url": "https://github.com/bootlin/buildroot.git",
        },
    }
    for repo_name, repo_dict in repo_version_map.items():
        url = config_dict.get(repo_dict["url"])
        version = config_dict.get(repo_dict["version"])

        if not (url and version):
            continue

        results[repo_name] = {"url": url, "version": version}

    return results


def _setup_image_build():
    # We need this for dynamic docstring in `inv image-build -h`
    image_build.__doc__ = f"""Available configs: \n{"\n".join(f"- {path}" for path in _get_config_paths())}
    """


def _setup_image_configure():
    # We need this for dynamic docstring in `inv image-configure -h`
    image_configure.__doc__ = f"""Available configs: \n{"\n".join(f"- {path}" for path in _get_config_paths())}
    """


def _get_config_paths():
    configs_paths = []
    for path in os.listdir(CONFIG_PATH):
        if "~" in path or "#" in path:
            continue
            
        if path in [".", "..", ".gitkeep"]:
            continue
            
        configs_paths.append(path)
        
    return configs_paths


_setup_image_build()

_setup_image_configure()
