#!/bin/bash

set -xeu

INPUT_PATH=$1
PAGE=$2
PAGE_TITLE=$(xmlstarlet sel -t -v "/mxfile/diagram[$PAGE]/@name" "$INPUT_PATH")
OUTPUT_NAME=$(echo $PAGE_TITLE).png
OUTPUT_PATH="$(dirname $INPUT_PATH)/$OUTPUT_NAME"

trap "rm -rf $OUTPUT_NAME" INT QUIT TERM EXIT

flatpak run com.jgraph.drawio.desktop \
    --export \
    --format png \
    --scale 3 \
    --page-index "$PAGE" \
    --output "$OUTPUT_NAME" \
    "$INPUT_PATH"

magick "$OUTPUT_NAME" \
    -background white \
    -gravity center \
    -bordercolor white \
    -border 100x100 \
    $OUTPUT_PATH
