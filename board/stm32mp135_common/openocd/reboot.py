import copy

from invoke import Context

from priv import CMD

def openocd_reboot(c):
    commands = ["init", "reset run", "shutdown"]
    cmd = copy.copy(CMD)

    if commands:
        cmd += " " + " ".join(f"-c '{command}'" for command in commands)

    c.run(cmd, pty=True)

if __name__ == "__main__":
    c = Context()
    openocd_reboot(c)
    
