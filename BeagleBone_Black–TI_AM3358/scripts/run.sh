#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

BINARY="$PROJECT_ROOT/output/host/bin/boxes"

echo "=========================================="
echo " Box Storage System - Run"
echo "=========================================="

if [ ! -f "$BINARY" ]; then
    echo "[ERROR] Application binary not found:"
    echo "  $BINARY"
    echo
    echo "Build first:"
    echo "  ./scripts/build.sh"
    exit 1
fi

echo "[INFO] Binary:"
echo "  $BINARY"

echo
echo "[INFO] Starting application..."
echo

"$BINARY" "$@"
