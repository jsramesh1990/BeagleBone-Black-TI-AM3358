#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

OUTPUT_TARGET="$PROJECT_ROOT/output/target"
SD_ROOTFS="$PROJECT_ROOT/deployment/sd-card/rootfs"

LOG_DIR="$PROJECT_ROOT/output/logs/deployment"

mkdir -p "$LOG_DIR"

echo "=========================================="
echo " BeagleBone Black - Deployment"
echo "=========================================="

DEPLOY_LOG="$LOG_DIR/sd-card-deploy.log"

echo "Deployment started: $(date)" | tee "$DEPLOY_LOG"

if [ ! -d "$OUTPUT_TARGET" ]; then
    echo "[ERROR] Target output does not exist."
    echo "Run:"
    echo "  ./scripts/build.sh"
    exit 1
fi

mkdir -p "$SD_ROOTFS/usr/bin"
mkdir -p "$SD_ROOTFS/etc/box-storage"
mkdir -p "$SD_ROOTFS/etc/systemd/system"

echo
echo "[1/4] Installing application..."

if [ -f "$OUTPUT_TARGET/bin/boxes" ]; then

    cp "$OUTPUT_TARGET/bin/boxes" \
       "$SD_ROOTFS/usr/bin/boxes"

    chmod +x "$SD_ROOTFS/usr/bin/boxes"

    echo "[PASS] boxes installed."
else
    echo "[ERROR] boxes binary not found."
    exit 1
fi

echo
echo "[2/4] Installing configuration..."

if [ -f "$OUTPUT_TARGET/config/boxes.conf" ]; then
    cp "$OUTPUT_TARGET/config/boxes.conf" \
       "$SD_ROOTFS/etc/box-storage/"
fi

if [ -f "$OUTPUT_TARGET/config/logging.conf" ]; then
    cp "$OUTPUT_TARGET/config/logging.conf" \
       "$SD_ROOTFS/etc/box-storage/"
fi

echo "[PASS] Configuration installed."

echo
echo "[3/4] Installing systemd service..."

if [ -f "$OUTPUT_TARGET/systemd/box-storage.service" ]; then

    cp "$OUTPUT_TARGET/systemd/box-storage.service" \
       "$SD_ROOTFS/etc/systemd/system/"

    echo "[PASS] systemd service installed."
fi

echo
echo "[4/4] Deployment contents:"

find "$SD_ROOTFS" -type f | sort | tee -a "$DEPLOY_LOG"

echo
echo "=========================================="
echo " DEPLOYMENT SUCCESSFUL"
echo "=========================================="

echo "RootFS:"
echo "  $SD_ROOTFS"

echo
echo "Deployment completed: $(date)" | tee -a "$DEPLOY_LOG"
