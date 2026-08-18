#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

APP_DIR="$PROJECT_ROOT/app"
OUTPUT_DIR="$PROJECT_ROOT/output"

TEST_DIR="$OUTPUT_DIR/host/tests"
LOG_DIR="$OUTPUT_DIR/logs/test"

mkdir -p "$TEST_DIR"
mkdir -p "$LOG_DIR"

CXX="${CXX:-g++}"

CXXFLAGS="-std=c++17 -Wall -Wextra -O2"
INCLUDES="-I$APP_DIR/include"

echo "=========================================="
echo " Box Storage System - Unit Tests"
echo "=========================================="

TEST_LOG="$LOG_DIR/test.log"

echo "Test started: $(date)" > "$TEST_LOG"

COMMON_SRC="
$APP_DIR/src/Date.cpp
$APP_DIR/src/Entry.cpp
$APP_DIR/src/StaticBox.cpp
$APP_DIR/src/DynamicBox.cpp
$APP_DIR/src/BoxManager.cpp
"

PASS=0
FAIL=0

build_and_run()
{
    TEST_NAME="$1"
    TEST_SOURCE="$2"

    echo
    echo "------------------------------------------"
    echo "Testing: $TEST_NAME"
    echo "------------------------------------------"

    BINARY="$TEST_DIR/$TEST_NAME"

    $CXX $CXXFLAGS $INCLUDES \
        $COMMON_SRC \
        "$TEST_SOURCE" \
        -o "$BINARY" 2>&1 | tee -a "$TEST_LOG"

    if "$BINARY" 2>&1 | tee -a "$TEST_LOG"; then
        echo "[PASS] $TEST_NAME"
        PASS=$((PASS + 1))
    else
        echo "[FAIL] $TEST_NAME"
        FAIL=$((FAIL + 1))
    fi
}

build_and_run \
    "test_date" \
    "$APP_DIR/tests/test_date.cpp"

build_and_run \
    "test_static_box" \
    "$APP_DIR/tests/test_static_box.cpp"

build_and_run \
    "test_dynamic_box" \
    "$APP_DIR/tests/test_dynamic_box.cpp"

build_and_run \
    "test_range_query" \
    "$APP_DIR/tests/test_range_query.cpp"

echo
echo "=========================================="
echo " TEST SUMMARY"
echo "=========================================="

echo "Passed : $PASS"
echo "Failed : $FAIL"

if [ "$FAIL" -ne 0 ]; then
    echo
    echo "[FAIL] Unit testing failed."
    exit 1
fi

echo
echo "[PASS] All unit tests passed."
