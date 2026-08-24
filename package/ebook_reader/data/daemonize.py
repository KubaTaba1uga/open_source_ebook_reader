import argparse
import subprocess
import signal
import atexit
import sys
import os
from datetime import datetime, time

process = None
log = None
is_done = False

def main():
    global process
    global log
    
    args = args_parse()
    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)
    atexit.register(exit_handler)


    log_cleanup = None
    if args.log:
        log = open(args.log, "a")
        log_cleanup = lambda: log.close()
    else:
        log = sys.stdout

    try:
        while not is_done:
            cmd = [args.exec] + args.args 
            log_write(log, f"Starting process, cmd={' '.join(cmd)}\n")            
            process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True
            )
            
            for line in process.stdout:
                log_write(log, line)
                
            returncode = process.wait()
            process = None
            log_write(log, f"Process finished, return code={returncode}\n")
            
    finally:
        if log_cleanup:
            log_cleanup()


def args_parse():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "exec",
        help="Executable to run",
    )

    parser.add_argument(
        "--log",
        help="Log file",
    )

    parser.add_argument(
        "args",
        nargs=argparse.REMAINDER,
        help="Arguments passed to the executable",
    )
    
    args = parser.parse_args()

    print("CLI args:")
    for name, value in vars(args).items():
        print(f"  {name}: {value}")
    print()

    return args


def sig_handler(signum, _):
    signame = signal.Signals(signum).name
    log_write(log, f"Signal handler called with signal {signame} ({signum})\n")
    exit_handler()


def exit_handler():
    global is_done
    
    if process is not None:
        process.send_signal(signal.SIGINT)
        process.wait()
        
    is_done = True
        
def log_write(log, text):
    now = datetime.now().isoformat()
    pid = process.pid if process else os.getpid()
    text = f"[{now}]({pid}): {text}"
    print(text, end="")
    log.write(text)
    log.flush()

if __name__ == "__main__":
    main()
