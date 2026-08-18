#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

OUTPUT_DIR="$PROJECT_ROOT/output"

echo "=========================================="
echo " Cleaning Project"
echo "=========================================="

if [ -d "$OUTPUT_DIR" ]; then
    echo "[INFO] Removing output directory..."

    rm -rf "$OUTPUT_DIR"

    echo "[PASS] Output directory removed."
else
    echo "[INFO] Output directory does not exist."
fi

echo
echo "[INFO] Cleaning complete."
