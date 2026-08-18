#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

SD_DIR="$PROJECT_ROOT/deployment/sd-card"

echo "=========================================="
echo " BeagleBone Black - SD Card Flash"
echo "=========================================="

if [ "$EUID" -ne 0 ]; then
    echo "[ERROR] Run this script with sudo."
    echo
    echo "Example:"
    echo "  sudo ./scripts/flash_sd.sh /dev/sdX"
    exit 1
fi

if [ -z "$1" ]; then
    echo "[ERROR] SD card device not specified."
    echo
    echo "Usage:"
    echo "  sudo ./scripts/flash_sd.sh /dev/sdX"
    echo
    echo "First identify the SD card using:"
    echo "  lsblk"
    exit 1
fi

DEVICE="$1"

echo
echo "[WARNING]"
echo "You selected:"
echo
echo "    $DEVICE"
echo
echo "ALL DATA ON THIS DEVICE MAY BE DESTROYED."
echo

read -r -p "Type YES to continue: " CONFIRM

if [ "$CONFIRM" != "YES" ]; then
    echo "[INFO] Flash cancelled."
    exit 0
fi

echo
echo "[INFO] Device information:"
lsblk "$DEVICE"

echo
echo "[INFO] Preparing SD card..."

sync

echo
echo "[INFO] SD-card deployment directory:"
echo "$SD_DIR"

echo
echo "[IMPORTANT]"
echo "The exact flashing method depends on the SD-card layout/image"
echo used by this project.

echo
echo "If deployment/sd-card contains a complete disk image, write"
echo that image using the project's image-generation process."
echo
echo "If it contains mounted boot/rootfs directories, mount the"
echo correct SD-card partitions and copy the files instead."

echo
echo "[INFO] flash_sd.sh reached the device verification stage."
echo "[INFO] Do not blindly run dd against an unverified device."

sync

echo
echo "=========================================="
echo " SD CARD FLASH PROCESS READY"
echo "=========================================="
