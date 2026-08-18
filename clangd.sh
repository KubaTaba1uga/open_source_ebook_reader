#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" >/dev/null && pwd)"

clangd-19 --log=verbose --pretty --compile-commands-dir=$SCRIPT_DIR/build 2> clangd.log
