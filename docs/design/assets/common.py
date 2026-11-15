import subprocess
import time


def cmd(args):
    pargs = args
    if type(args) is str:
        args = args.split(" ")
    else:
        pargs = " ".join(args)

    print("✦ ❯", pargs)

    proc = subprocess.Popen(
        args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
    )

    results = []
    for line in proc.stdout:
        results.append(line)
        print(line, end="")

    proc.wait()
    if proc.returncode != 0:
        raise OSError(f"Command failed: {args}")

    return "\n".join(results)


def uart_cmd(args):
    pargs = args
    if type(args) is str:
        args = args.split(" ")
    else:
        pargs = " ".join(args)

    print("✦ ❯", pargs)

    proc = subprocess.Popen(
        args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
    )

    results = []
    for line in proc.stdout:
        results.append(line)
        print(line, end="")
        if line.startswith("#"):
            proc.kill()
            break

    proc.wait()
    if proc.returncode != 0:
        raise OSError(f"Command failed: {args}")

    return "\n".join(results)


import pexpect
import sys

ctx = {"status": "none", "dev": None}


def uart_cmd_spawn(device):
    if ctx["status"] == "none":
        tio_cmd = f"tio {device}"
        child = pexpect.spawn(
            tio_cmd, encoding="utf-8", codec_errors="ignore", timeout=10
        )
        child.logfile_read = sys.stdout
        child.expect(r"Connected", timeout=5)
        child.expect([r"login:", pexpect.TIMEOUT], timeout=30)
        child.sendline("root")
        ctx["shell"] = child
        ctx["dev"] = device
        ctx["status"] = "logged_in"
        time.sleep(0.5)
        print("Logged in")


def uart_cmd_destroy():
    global ctx
    ctx = {"status": "none", "dev": None}


def uart_cmd(command, timeout=120):
    """
    Spawns tio, sends a command, and captures the output.
    """
    shell = ctx.get("shell")
    if ctx["status"] == "none":
        raise ValueError()
    elif ctx["status"] == "logged_in":
        shell.expect([r"#", pexpect.TIMEOUT], timeout=timeout)
        shell.sendline(command)
        shell.expect([r"#", pexpect.TIMEOUT], timeout=timeout)
        ctx["status"] = "in_program"
    elif ctx["status"] == "in_program":
        shell.sendline(command)
        shell.expect([r"#", pexpect.TIMEOUT], timeout=timeout)

    time.sleep(0.5)        
    try:
        output = shell.before.replace("\r\n", "\n").split("\n")
        output.pop(0)
        output.pop()

        
        return [line.strip() for line in output]
    except:
        return []


# .replace(f"{command}\n", "").split("\n")
#     elif idx == 1:
#         child.sendline(command)
#     else:
#         child.send("\x03")
#         raise ValueError()

#     child.expect([r"[\$#]\s*$", pexpect.TIMEOUT], timeout=5)

#     # Wait for tio to connect or for a prompt/stabilization period
#     # (tio usually prints a connection status message like "[connected ...]")
#     child.expect(r"\[connected", timeout=timeout)

#     # Send a newline or clear out any startup chatter if necessary
#     child.sendline("")

#     # Send your actual command
#     print(f"\n[Sending command]: {command}")
#     child.sendline(command)

#     # Wait for completion or expect a response marker / prompt
#     # (Change "OK" or ">" to whatever your device outputs when finished)
#     child.expect(["OK", pexpect.TIMEOUT], timeout=timeout)

#     # Grab whatever buffer was read up to this point
#     output = child.before
#     return output

# except pexpect.TIMEOUT:
#     print("\n[Error]: Command timed out waiting for device response.")
#     return child.before
# finally:
#     # Gracefully exit tio using its shortcut sequence (Ctrl+T followed by q)
#     # \x14 is Ctrl+T
#     try:
#         child.send("\x14q")
#         child.close()
#     except Exception:
#         pass
