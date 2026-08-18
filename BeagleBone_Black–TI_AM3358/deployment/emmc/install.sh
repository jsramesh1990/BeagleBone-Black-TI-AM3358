#!/bin/bash

set -e

TARGET="/opt/box-storage"

echo "Installing Box Storage application..."

sudo mkdir -p "$TARGET/bin"
sudo mkdir -p "$TARGET/config"

sudo mkdir -p /var/lib/box-storage
sudo mkdir -p /var/log/box-storage

echo "Installing binary..."

sudo cp ../../bin/boxes \
    "$TARGET/bin/boxes"

echo "Installing configuration..."

sudo cp ../../config/boxes.conf \
    "$TARGET/config/boxes.conf"

sudo cp ../../config/logging.conf \
    "$TARGET/config/logging.conf"

echo "Installing systemd service..."

sudo cp ../systemd/box-storage.service \
    /etc/systemd/system/box-storage.service

sudo chmod +x \
    "$TARGET/bin/boxes"

sudo systemctl daemon-reload

sudo systemctl enable box-storage.service

echo
echo "Installation completed."

echo
echo "Application:"
echo "  $TARGET/bin/boxes"

echo
echo "Configuration:"
echo "  $TARGET/config/"

echo
echo "Service:"
echo "  box-storage.service"
