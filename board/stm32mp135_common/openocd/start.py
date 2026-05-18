import sys

from invoke import Context

from priv import CMD

def openocd_run(c):
    c.run(CMD, pty=True)

if __name__ == "__main__":
    c = Context()
    openocd_run(c)
    
