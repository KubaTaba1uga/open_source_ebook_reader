#!/bin/bash

set -xeu

PAGE=$1

# Normal
# /tmp/pdf_poppler --scale 2 /mnt/sdcard/Operating_Systems_From_0_to_1.pdf output_${PAGE}.png --page $PAGE

# Big
# /tmp/pdf_poppler --scale 8 /mnt/sdcard/Operating_Systems_From_0_to_1.pdf /tmp/output_${PAGE}_big.png --page $PAGE
# magick /tmp/output_${PAGE}_big.png -colorspace Gray -ordered-dither o8x8,2 /tmp/output_${PAGE}_big_bw.png
# magick /tmp/output_${PAGE}_big_bw.png -resize 25% output_${PAGE}_bw.png

# Medium
/tmp/pdf_poppler --scale 5 /mnt/sdcard/Operating_Systems_From_0_to_1.pdf /tmp/output_${PAGE}_big.png --page $PAGE
magick /tmp/output_${PAGE}_big.png -colorspace Gray -ordered-dither o8x8,2 /tmp/output_${PAGE}_big_bw.png
magick /tmp/output_${PAGE}_big_bw.png -resize 40% output_${PAGE}_bw.png

# Small
# /tmp/pdf_poppler --scale 4 /mnt/sdcard/Operating_Systems_From_0_to_1.pdf /tmp/output_${PAGE}_big.png --page $PAGE
# magick /tmp/output_${PAGE}_big.png -colorspace Gray -ordered-dither o8x8,2 /tmp/output_${PAGE}_big_bw.png
# magick /tmp/output_${PAGE}_big_bw.png -resize 50% output_${PAGE}_bw.png
