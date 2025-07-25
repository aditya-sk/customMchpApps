#!/bin/sh

APP_DIR="$(pwd)"
BUILD_DIR="$APP_DIR/build"
BOARD_NAME="same54_xpro"

# Check if the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
    echo "Directory $BUILD_DIR created."
else
    echo "Directory $BUILD_DIR already exists."
fi

# Run and build application in Linux Host Machine
west build -p always -b "$BOARD_NAME" "$APP_DIR" -d "$BUILD_DIR"

# Flash the image to the target/board ==> same54_xpro development board
west flash -d "$BUILD_DIR"