For your **BeagleBone Black – TI AM3358 / Box Storage System**, I recommend having **both `Makefile` and `CMakeLists.txt`**, but they should have clearly different purposes:

* **Makefile** → simple developer commands: build, test, clean, deploy, run.
* **CMakeLists.txt** → actual C++ build definition and cross-compilation support.
* **Yocto** → production embedded Linux image generation.

A clean overall structure is:

```text
BeagleBone_Black-TI_AM3358/
│
├── Makefile
├── CMakeLists.txt
│
├── app/
│   ├── include/
│   │   ├── BoxManager.hpp
│   │   ├── Date.hpp
│   │   ├── DynamicBox.hpp
│   │   ├── Entry.hpp
│   │   └── StaticBox.hpp
│   │
│   ├── src/
│   │   ├── BoxManager.cpp
│   │   ├── Date.cpp
│   │   ├── DynamicBox.cpp
│   │   ├── Entry.cpp
│   │   ├── StaticBox.cpp
│   │   └── main.cpp
│   │
│   └── tests/
│       ├── test_date.cpp
│       ├── test_dynamic_box.cpp
│       ├── test_range_query.cpp
│       └── test_static_box.cpp
│
├── config/
│   ├── boxes.conf
│   └── logging.conf
│
├── scripts/
│   ├── build.sh
│   ├── clean.sh
│   ├── deploy.sh
│   ├── flash_sd.sh
│   ├── run.sh
│   └── test.sh
│
├── deployment/
│   ├── emmc/
│   ├── sd-card/
│   └── systemd/
│
├── docs/
│   ├── architecture.md
│   ├── boot_flow.md
│   ├── build_flow.md
│   ├── debugging.md
│   ├── storage_flow.md
│   └── testing.md
│
├── output/
│   ├── host/
│   ├── logs/
│   └── target/
│
└── yocto/
    └── meta-box-storage/
```

# 1. Overall `CMakeLists.txt`

Put this at the project root:

```cmake
cmake_minimum_required(VERSION 3.16)

project(
    BoxStorage
    VERSION 1.0.0
    DESCRIPTION "Date Indexed Box Storage System"
    LANGUAGES CXX
)

# ============================================================
# C++ Configuration
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warning flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# Directories
# ============================================================

set(APP_DIR       ${CMAKE_SOURCE_DIR}/app)
set(INCLUDE_DIR   ${APP_DIR}/include)
set(SRC_DIR       ${APP_DIR}/src)
set(TEST_DIR      ${APP_DIR}/tests)

# ============================================================
# Application Source
# ============================================================

set(APP_SOURCES
    ${SRC_DIR}/BoxManager.cpp
    ${SRC_DIR}/Date.cpp
    ${SRC_DIR}/DynamicBox.cpp
    ${SRC_DIR}/Entry.cpp
    ${SRC_DIR}/StaticBox.cpp
)

# ============================================================
# Application Library
# ============================================================

add_library(box_storage_lib STATIC
    ${APP_SOURCES}
)

target_include_directories(
    box_storage_lib
    PUBLIC
        ${INCLUDE_DIR}
)

# ============================================================
# Main Application
# ============================================================

add_executable(
    boxes
    ${SRC_DIR}/main.cpp
)

target_link_libraries(
    boxes
    PRIVATE
        box_storage_lib
)

# ============================================================
# Installation
# ============================================================

include(GNUInstallDirs)

install(
    TARGETS boxes
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY ${INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/box-storage
)

# ============================================================
# Configuration Files
# ============================================================

install(
    FILES
        ${CMAKE_SOURCE_DIR}/config/boxes.conf
        ${CMAKE_SOURCE_DIR}/config/logging.conf
    DESTINATION /etc/box-storage
)

# ============================================================
# Testing
# ============================================================

enable_testing()

# ------------------------------------------------------------
# Date Test
# ------------------------------------------------------------

add_executable(
    test_date
    ${TEST_DIR}/test_date.cpp
)

target_link_libraries(
    test_date
    PRIVATE
        box_storage_lib
)

add_test(
    NAME DateTest
    COMMAND test_date
)

# ------------------------------------------------------------
# StaticBox Test
# ------------------------------------------------------------

add_executable(
    test_static_box
    ${TEST_DIR}/test_static_box.cpp
)

target_link_libraries(
    test_static_box
    PRIVATE
        box_storage_lib
)

add_test(
    NAME StaticBoxTest
    COMMAND test_static_box
)

# ------------------------------------------------------------
# DynamicBox Test
# ------------------------------------------------------------

add_executable(
    test_dynamic_box
    ${TEST_DIR}/test_dynamic_box.cpp
)

target_link_libraries(
    test_dynamic_box
    PRIVATE
        box_storage_lib
)

add_test(
    NAME DynamicBoxTest
    COMMAND test_dynamic_box
)

# ------------------------------------------------------------
# Range Query Test
# ------------------------------------------------------------

add_executable(
    test_range_query
    ${TEST_DIR}/test_range_query.cpp
)

target_link_libraries(
    test_range_query
    PRIVATE
        box_storage_lib
)

add_test(
    NAME RangeQueryTest
    COMMAND test_range_query
)
```

---

# 2. Build with CMake

From the project root:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

You get something like:

```text
build/
├── boxes
├── libbox_storage_lib.a
├── test_date
├── test_static_box
├── test_dynamic_box
└── test_range_query
```

Run:

```bash
./build/boxes
```

Run tests:

```bash
cd build
ctest --output-on-failure
```

---

# 3. Now the overall `Makefile`

The Makefile should act as a **developer-friendly wrapper around CMake**.

Create:

```text
BeagleBone_Black-TI_AM3358/Makefile
```

```makefile
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
```

---

# 4. Now your build commands become very simple

Instead of remembering complicated commands:

```bash
cmake -S . -B build
cmake --build build
ctest
```

you can simply use:

```bash
make
```

or:

```bash
make build
```

### Test

```bash
make test
```

### Run

```bash
make run
```

### Demo

```bash
make demo
```

### Clean

```bash
make clean
```

### Complete clean

```bash
make distclean
```

---

# 5. Important: Makefile vs CMake

Think of them like this:

```text
                    Makefile
                       |
                       | "What do I want?"
                       |
        +--------------+--------------+
        |              |              |
      build          test           clean
        |              |              |
        +--------------+--------------+
                       |
                       v
                     CMake
                       |
                       | "How do I compile?"
                       |
        +--------------+--------------+
        |              |              |
     Compiler       Sources       Libraries
        |              |              |
        +--------------+--------------+
                       |
                       v
                  C++ Executable
```

### CMake knows:

```text
Which .cpp files?
Which headers?
Which libraries?
Which compiler flags?
Which tests?
Where to install?
```

### Makefile knows:

```text
make build
make test
make run
make deploy
make image
make clean
```

---

# 6. CMake → BeagleBone cross compilation

This is where your project becomes more embedded-oriented.

Your PC might be:

```text
Ubuntu x86_64
```

but BeagleBone Black is:

```text
TI AM3358
ARM Cortex-A8
```

Therefore:

```text
PC
 |
 | x86_64 compiler
 |
 +----> Host binary
```

is different from:

```text
PC
 |
 | ARM cross compiler
 |
 +----> ARM binary
             |
             v
       BeagleBone Black
```

For example, with an ARM GCC toolchain:

```bash
arm-linux-gnueabihf-g++
```

you could configure:

```bash
cmake \
    -S . \
    -B build-arm \
    -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ \
    -DCMAKE_BUILD_TYPE=Release
```

Then:

```bash
cmake --build build-arm
```

The resulting `boxes` executable is intended for the ARM target, not your x86 PC.

---

# 7. Better approach with a CMake toolchain file

For your project, I recommend eventually adding:

```text
cmake/
└── toolchain-arm.cmake
```

Example:

```cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER
    arm-linux-gnueabihf-gcc
)

set(CMAKE_CXX_COMPILER
    arm-linux-gnueabihf-g++
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

Then:

```bash
cmake \
    -S . \
    -B build-arm \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm.cmake
```

and:

```bash
cmake --build build-arm
```

---

# 8. But Yocto changes this

For your **final production BSP**, don't manually choose:

```text
arm-linux-gnueabihf-g++
```

inside the application build.

Yocto provides the cross compiler, sysroot, flags and environment.

The production flow becomes:

```text
                 Your C++ source
                       |
                       v
                box-storage.bb
                       |
                       v
                    BitBake
                       |
          +------------+------------+
          |                         |
          v                         v
      Compiler                  Sysroot
          |                         |
          +------------+------------+
                       |
                       v
                ARM executable
                       |
                       v
                    Package
                       |
                       v
                    RootFS
                       |
                       v
                 Yocto Image
                       |
                       v
                 SD Card / eMMC
                       |
                       v
              BeagleBone AM3358
```

So you should understand **three levels**:

### Level 1 — Simple C++ development

```bash
g++ ...
```

### Level 2 — CMake project

```bash
cmake ...
cmake --build ...
ctest
```

### Level 3 — Embedded production

```bash
bitbake box-storage
bitbake <image>
```

---

# 9. Final build architecture for your project

This is the structure I recommend you use:

```text
                    BeagleBone_Black-TI_AM3358
                               |
          +--------------------+--------------------+
          |                    |                    |
          v                    v                    v
        app/               scripts/              docs/
          |                    |                    |
       C++ code          Automation             Documentation
          |                    |
          +---------+----------+
                    |
                    v
              CMakeLists.txt
                    |
                    v
                 Makefile
                    |
          +---------+---------+
          |                   |
          v                   v
      Host Build          ARM Build
          |                   |
          v                   v
   output/host/        output/target/
          |                   |
          +---------+---------+
                    |
                    v
                deployment/
                    |
                    v
                  SD/eMMC
                    |
                    v
             BeagleBone Black
```

And separately:

```text
                 app/
                  |
                  v
          meta-box-storage
                  |
                  v
             box-storage.bb
                  |
                  v
               BitBake
                  |
                  v
             Yocto Image
                  |
                  v
             SD Card/eMMC
                  |
                  v
          BeagleBone AM3358
```

**One important correction to the earlier structure:** don't make `Makefile`, CMake, your shell scripts, and Yocto all compile the same source independently in production. Use **CMake as the authoritative C++ build definition**, `Makefile` as the convenient developer entry point, and **Yocto as the authoritative target/image build system**. This avoids the project having three different definitions of how `BoxManager.cpp`, `Date.cpp`, etc. are compiled.

