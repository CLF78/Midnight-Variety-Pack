#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <path-to-dolphin-emulator> <path-to-preset.json>"
    exit 1
fi

# Assign arguments to variables
DOLPHIN="$1"
PRESET_JSON="$2"

# Check if the executable file exists
if [ ! -f "$DOLPHIN" ]; then
    echo "Error: Dolphin Emulator could not be found at '$DOLPHIN'."
    exit 1
fi

# Check if the preset json file exists
if [ ! -f "$PRESET_JSON" ]; then
    echo "Error: the preset JSON '$PRESET_JSON' does not exist."
    exit 1
fi

# Kill any Dolphin instance already running
pkill -x "$(basename "$DOLPHIN")"

# Run Dolphin with the preset json file
"$DOLPHIN" -e "$PRESET_JSON"
exit 0
