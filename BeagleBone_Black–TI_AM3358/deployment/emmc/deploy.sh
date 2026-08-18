#!/bin/bash

set -e

APP_NAME="boxes"
TARGET_DIR="/opt/boxes"

echo "===================================="
echo " BeagleBone Black eMMC Deployment"
echo "===================================="

echo "[1/5] Creating target directory..."

sudo mkdir -p "${TARGET_DIR}/bin"
sudo mkdir -p "${TARGET_DIR}/config"
sudo mkdir -p /var/lib/boxes
sudo mkdir -p /var/log/boxes

echo "[2/5] Installing application..."

sudo cp ../../bin/boxes \
    "${TARGET_DIR}/bin/boxes"

echo "[3/5] Installing configuration..."

sudo cp ../../config/boxes.conf \
    "${TARGET_DIR}/config/boxes.conf"

sudo cp ../../config/logging.conf \
    "${TARGET_DIR}/config/logging.conf"

echo "[4/5] Setting permissions..."

sudo chmod +x \
    "${TARGET_DIR}/bin/boxes"

echo "[5/5] Deployment completed."

echo
echo "Application:"
echo "  ${TARGET_DIR}/bin/boxes"

echo
echo "Configuration:"
echo "  ${TARGET_DIR}/config/"

echo
echo "Data:"
echo "  /var/lib/boxes"

echo
echo "Logs:"
echo "  /var/log/boxes"
