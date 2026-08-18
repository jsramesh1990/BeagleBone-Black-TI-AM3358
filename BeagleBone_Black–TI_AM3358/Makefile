# ============================================================
# BeagleBone Black - TI AM3358
# Box Storage System
# ============================================================

PROJECT_NAME := box-storage

BUILD_DIR := build
OUTPUT_DIR := output
HOST_DIR := $(OUTPUT_DIR)/host
TARGET_DIR := $(OUTPUT_DIR)/target
LOG_DIR := $(OUTPUT_DIR)/logs

CMAKE := cmake

# ============================================================
# Default Target
# ============================================================

.PHONY: all

all: build

# ============================================================
# Configure
# ============================================================

.PHONY: configure

configure:
	@echo "=========================================="
	@echo " Configuring CMake"
	@echo "=========================================="

	mkdir -p $(BUILD_DIR)

	$(CMAKE) \
		-S . \
		-B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release

# ============================================================
# Build
# ============================================================

.PHONY: build

build: configure
	@echo "=========================================="
	@echo " Building Box Storage"
	@echo "=========================================="

	$(CMAKE) \
		--build $(BUILD_DIR) \
		--parallel

	@mkdir -p $(HOST_DIR)/bin
	@mkdir -p $(TARGET_DIR)/bin
	@mkdir -p $(LOG_DIR)/build

	cp $(BUILD_DIR)/boxes \
	   $(HOST_DIR)/bin/boxes

	cp $(BUILD_DIR)/boxes \
	   $(TARGET_DIR)/bin/boxes

	@echo
	@echo "Build completed."
	@echo "Host binary:"
	@echo "  $(HOST_DIR)/bin/boxes"
	@echo
	@echo "Target binary:"
	@echo "  $(TARGET_DIR)/bin/boxes"

# ============================================================
# Debug Build
# ============================================================

.PHONY: debug

debug:
	@echo "Building Debug version..."

	mkdir -p $(BUILD_DIR)

	$(CMAKE) \
		-S . \
		-B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Debug

	$(CMAKE) \
		--build $(BUILD_DIR) \
		--parallel

# ============================================================
# Test
# ============================================================

.PHONY: test

test: build
	@echo "=========================================="
	@echo " Running Unit Tests"
	@echo "=========================================="

	mkdir -p $(LOG_DIR)/test

	cd $(BUILD_DIR) && \
	ctest --output-on-failure

# ============================================================
# Run
# ============================================================

.PHONY: run

run: build
	@echo "=========================================="
	@echo " Running Application"
	@echo "=========================================="

	./$(HOST_DIR)/bin/boxes

# ============================================================
# Demo
# ============================================================

.PHONY: demo

demo: build
	./$(HOST_DIR)/bin/boxes demo

# ============================================================
# Install Target Files
# ============================================================

.PHONY: deploy

deploy: build
	@echo "=========================================="
	@echo " Deploying Target Files"
	@echo "=========================================="

	./scripts/deploy.sh

# ============================================================
# SD Card
# ============================================================

.PHONY: flash

flash: deploy
	@echo "=========================================="
	@echo " Flash SD Card"
	@echo "=========================================="

	@echo "Use:"
	@echo "sudo ./scripts/flash_sd.sh /dev/sdX"

# ============================================================
# Yocto
# ============================================================

.PHONY: yocto

yocto:
	@echo "=========================================="
	@echo " Yocto Build"
	@echo "=========================================="

	cd yocto/build && \
	bitbake box-storage

# ============================================================
# Yocto Image
# ============================================================

.PHONY: image

image:
	@echo "=========================================="
	@echo " Building Yocto Image"
	@echo "=========================================="

	cd yocto/build && \
	bitbake core-image-minimal

# ============================================================
# Clean
# ============================================================

.PHONY: clean

clean:
	@echo "Cleaning CMake build..."

	rm -rf $(BUILD_DIR)

# ============================================================
# Distclean
# ============================================================

.PHONY: distclean

distclean:
	@echo "=========================================="
	@echo " Deep Clean"
	@echo "=========================================="

	rm -rf $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)

# ============================================================
# Help
# ============================================================

.PHONY: help

help:
	@echo
	@echo "BeagleBone Black - Box Storage System"
	@echo
	@echo "Available targets:"
	@echo
	@echo "  make configure   Configure CMake"
	@echo "  make build       Build application"
	@echo "  make debug       Debug build"
	@echo "  make test        Run unit tests"
	@echo "  make run         Run application"
	@echo "  make demo        Run demo"
	@echo "  make deploy      Prepare target filesystem"
	@echo "  make flash       Prepare SD-card flashing"
	@echo "  make yocto       Build Yocto recipe"
	@echo "  make image       Build Yocto image"
	@echo "  make clean       Remove CMake build"
	@echo "  make distclean   Remove all generated output"
	@echo
