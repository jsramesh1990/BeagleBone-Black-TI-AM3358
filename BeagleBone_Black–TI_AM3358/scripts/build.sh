#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

APP_DIR="$PROJECT_ROOT/app"
OUTPUT_DIR="$PROJECT_ROOT/output"
HOST_DIR="$OUTPUT_DIR/host"
TARGET_DIR="$OUTPUT_DIR/target"
LOG_DIR="$OUTPUT_DIR/logs/build"

CXX="${CXX:-g++}"

CXXFLAGS="-std=c++17 -Wall -Wextra -O2"
INCLUDES="-I$APP_DIR/include"

echo "=========================================="
echo " BeagleBone Black Box Storage - Build"
echo "=========================================="

echo "[INFO] Project root : $PROJECT_ROOT"
echo "[INFO] Compiler     : $CXX"

mkdir -p "$HOST_DIR/bin"
mkdir -p "$HOST_DIR/obj"
mkdir -p "$TARGET_DIR/bin"
mkdir -p "$LOG_DIR"

BUILD_LOG="$LOG_DIR/build.log"

echo "[INFO] Build started: $(date)" | tee "$BUILD_LOG"

echo
echo "[1/4] Compiling application..."

$CXX $CXXFLAGS $INCLUDES \
    "$APP_DIR/src/Date.cpp" \
    "$APP_DIR/src/Entry.cpp" \
    "$APP_DIR/src/StaticBox.cpp" \
    "$APP_DIR/src/DynamicBox.cpp" \
    "$APP_DIR/src/BoxManager.cpp" \
    "$APP_DIR/src/main.cpp" \
    -o "$HOST_DIR/bin/boxes" 2>&1 | tee -a "$BUILD_LOG"

echo "[PASS] Host application built."

echo
echo "[2/4] Preparing target application..."

cp "$HOST_DIR/bin/boxes" "$TARGET_DIR/bin/boxes"

echo "[PASS] Target application prepared."

echo
echo "[3/4] Installing configuration files..."

mkdir -p "$TARGET_DIR/config"

if [ -f "$PROJECT_ROOT/config/boxes.conf" ]; then
    cp "$PROJECT_ROOT/config/boxes.conf" \
       "$TARGET_DIR/config/"
fi

if [ -f "$PROJECT_ROOT/config/logging.conf" ]; then
    cp "$PROJECT_ROOT/config/logging.conf" \
       "$TARGET_DIR/config/"
fi

echo "[PASS] Configuration copied."

echo
echo "[4/4] Installing systemd service..."

mkdir -p "$TARGET_DIR/systemd"

if [ -f "$PROJECT_ROOT/deployment/systemd/box-storage.service" ]; then
    cp "$PROJECT_ROOT/deployment/systemd/box-storage.service" \
       "$TARGET_DIR/systemd/"
fi

echo "[PASS] Systemd service prepared."

echo
echo "=========================================="
echo " BUILD SUCCESSFUL"
echo "=========================================="

echo "Binary:"
echo "  $HOST_DIR/bin/boxes"

echo
echo "Target files:"
find "$TARGET_DIR" -type f

echo
echo "[INFO] Build completed: $(date)" | tee -a "$BUILD_LOG"
