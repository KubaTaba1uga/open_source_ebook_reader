from invoke import Context
import os

def install_libgpiod(c):
    r = c.run("pkg-config --modversion libgpiod", warn=True)

    if r.ok and "1.6.5" in r.stdout.strip():
        return

    try:
        c.run(
            "curl https://mirrors.edge.kernel.org/pub/software/libs/libgpiod/libgpiod-1.6.5.tar.xz -o /tmp/libpiod.tar.xz"
        )
        c.run("tar -xf /tmp/libpiod.tar.xz -C /tmp/")
        with c.cd("/tmp/libgpiod-1.6.5"):
            c.run("./configure --prefix=/usr/")
            c.run("make")
            c.run("sudo make install")

    except Exception:
        raise

if __name__ == "__main__":
    c = Context()
    install_libgpiod(c)
