#!/bin/bash

# https://gist.github.com/arturo182/57ab066e6a4a36ee22979063e4d5cce1#file-kicad_mp4-sh

OUTPUT_DIR="mp4"
INPUT_FILE="HF-PA-v10.kicad_pcb"
ZOOM=0.7
WIDTH=2048
HEIGHT=2048
ROTATE_X=0
ROTATE_Z=45
ROTATION=360 # Total rotation angle
STEP=3 # Rotation step in degrees
FRAMERATE=15 # Framerate for the final video

mkdir -p $OUTPUT_DIR

let FRAMES=ROTATION/STEP
for ((i = 0; i < FRAMES; i++)); do
    ROTATE_Y=-$(($i * STEP))
    OUTPUT_PATH="$OUTPUT_DIR/frame$i.png"
    echo "Rendering frame $i ($ROTATE_Y degrees) to $OUTPUT_PATH"
    kicad-cli-nightly pcb render --rotate "$ROTATE_X,$ROTATE_Y,$ROTATE_Z" --zoom $ZOOM -w $WIDTH -h $HEIGHT --background opaque -o $OUTPUT_PATH $INPUT_FILE > /dev/null
done

# Combine frames into an MP4 with the specified framerate
echo "Combining frames into an MP4..."
ffmpeg -y -framerate $FRAMERATE -i "$OUTPUT_DIR/frame%d.png" -c:v libx265 -r 30 -pix_fmt yuv420p "$OUTPUT_DIR/output.mp4"

echo "MP4 created successfully."
