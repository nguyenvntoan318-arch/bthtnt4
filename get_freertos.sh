#!/bin/bash
set -e

URL="https://github.com/FreeRTOS/FreeRTOS-Kernel.git"
DIR="FreeRTOS-Kernel"

if [ -f "$DIR/tasks.c" ] && [ -f "$DIR/include/FreeRTOS.h" ]; then
    echo "FreeRTOS-Kernel da ton tai."
    exit 0
fi

rm -rf "$DIR"
git clone --depth 1 "$URL" "$DIR"

echo "Da tai FreeRTOS-Kernel."
