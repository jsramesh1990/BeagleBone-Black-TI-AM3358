#!/bin/bash

set -e

SERVICE_NAME="boxes.service"

echo "Installing Box Storage systemd service..."

sudo cp boxes.service \
    /etc/systemd/system/${SERVICE_NAME}

sudo systemctl daemon-reload

sudo systemctl enable ${SERVICE_NAME}

echo
echo "Service installed successfully."

echo
echo "Start:"
echo "  sudo systemctl start ${SERVICE_NAME}"

echo
echo "Status:"
echo "  sudo systemctl status ${SERVICE_NAME}"

echo
echo "Logs:"
echo "  journalctl -u ${SERVICE_NAME}"
