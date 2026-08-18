#!/bin/bash

set -e

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <SD_DEVICE>"
    echo
    echo "Example:"
    echo "  $0 /dev/sdb"
    exit 1
fi

SD_DEVICE="$1"

echo "===================================="
echo " BeagleBone Black SD Deployment"
echo "===================================="

echo
echo "WARNING!"
echo "This operation targets:"
echo "  ${SD_DEVICE}"
echo
echo "Make sure this is the correct SD card."
echo

read -p "Continue? [y/N]: " answer

if [ "${answer}" != "y" ]; then
    echo "Deployment cancelled."
    exit 0
fi

echo
echo "Preparing SD-card deployment..."

echo "Application:"
echo "  ../../bin/boxes"

echo
echo "Configuration:"
echo "  ../../config/"

echo
echo "After the SD card is prepared:"
echo
echo "  1. Insert SD card into BeagleBone Black"
echo "  2. Configure boot mode"
echo "  3. Power on board"
echo "  4. U-Boot starts"
echo "  5. Linux kernel starts"
echo "  6. RootFS mounts"
echo "  7. systemd starts"
echo "  8. boxes application starts"

echo
echo "SD-card deployment preparation completed."
