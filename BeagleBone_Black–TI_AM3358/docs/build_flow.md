Yes. Add **`build_flow.md`** under `docs/`.

Your structure becomes:

```text
BeagleBone_Black-TI_AM3358/
└── docs/
    ├── architecture.md
    ├── boot_flow.md
    └── build_flow.md
```

This document should explain **source code → compilation → linking → ARM executable → rootfs → image → SD/eMMC → BeagleBone**.

# `docs/build_flow.md`

````markdown
# BeagleBone Black TI AM3358 Build Flow

## 1. Overview

This document describes how the Box Storage System is built,
cross-compiled, packaged, and deployed for the BeagleBone Black
using the TI AM3358 processor.

The complete build flow is:

```text
C++ Source
    |
    v
Preprocessing
    |
    v
Compilation
    |
    v
Object Files
    |
    v
Linking
    |
    v
ARM Executable
    |
    v
Unit Testing
    |
    v
Packaging
    |
    v
RootFS
    |
    v
Bootable Image
    |
    v
SD Card / eMMC
    |
    v
BeagleBone Black
````

---

# 2. Host and Target

The project has two different environments.

## Host

The development/build machine.

Example:

```text
Ubuntu Linux
x86_64
g++
make
git
```

## Target

The actual embedded board.

```text
BeagleBone Black
TI AM3358
ARM Cortex-A8
ARMv7-A
Embedded Linux
```

The host and target architectures are different.

```text
Host
x86_64
   |
   | Cross Compilation
   v
Target
ARM Cortex-A8
```

---

# 3. Project Structure

```text
BeagleBone_Black-TI_AM3358/
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
├── deployment/
│   ├── emmc/
│   ├── sd-card/
│   └── systemd/
│
├── docs/
│   ├── architecture.md
│   ├── boot_flow.md
│   └── build_flow.md
│
├── Makefile
└── README.md
```

---

# 4. Source Files

The main application source files are:

```text
app/src/
├── Date.cpp
├── Entry.cpp
├── StaticBox.cpp
├── DynamicBox.cpp
├── BoxManager.cpp
└── main.cpp
```

Corresponding headers:

```text
app/include/
├── Date.hpp
├── Entry.hpp
├── StaticBox.hpp
├── DynamicBox.hpp
└── BoxManager.hpp
```

The relationship is:

```text
Date.hpp
   ^
   |
Date.cpp

Entry.hpp
   ^
   |
Entry.cpp

StaticBox.hpp
   ^
   |
StaticBox.cpp

DynamicBox.hpp
   ^
   |
DynamicBox.cpp

BoxManager.hpp
   ^
   |
BoxManager.cpp

main.cpp
```

---

# 5. Preprocessing

The first stage of C++ compilation is preprocessing.

Example:

```bash
g++ -E app/src/main.cpp
```

The preprocessor handles:

```text
#include
#define
#ifdef
#ifndef
#if
#endif
```

For example:

```cpp
#include "BoxManager.hpp"
```

causes the contents of the header to be included during preprocessing.

Conceptually:

```text
main.cpp
   |
   +----> BoxManager.hpp
   |          |
   |          +----> StaticBox.hpp
   |          |
   |          +----> DynamicBox.hpp
   |
   v
Preprocessed source
```

---

# 6. Compilation

Each `.cpp` file is compiled separately.

Example:

```bash
g++ -std=c++17 -Wall -Wextra -O2 \
    -Iapp/include \
    -c app/src/Date.cpp \
    -o build/Date.o
```

The same process happens for all source files.

```text
Date.cpp
    |
    v
Date.o

Entry.cpp
    |
    v
Entry.o

StaticBox.cpp
    |
    v
StaticBox.o

DynamicBox.cpp
    |
    v
DynamicBox.o

BoxManager.cpp
    |
    v
BoxManager.o

main.cpp
    |
    v
main.o
```

---

# 7. Object Files

After compilation:

```text
build/
├── Date.o
├── Entry.o
├── StaticBox.o
├── DynamicBox.o
├── BoxManager.o
└── main.o
```

An object file contains compiled machine code and other
information required by the linker.

The application is not yet a complete executable.

---

# 8. Linking

The linker combines all object files.

```bash
g++ \
    build/Date.o \
    build/Entry.o \
    build/StaticBox.o \
    build/DynamicBox.o \
    build/BoxManager.o \
    build/main.o \
    -o build/boxes
```

Conceptually:

```text
Date.o
Entry.o
StaticBox.o
DynamicBox.o
BoxManager.o
main.o
    |
    v
  Linker
    |
    +---- C++ runtime
    +---- Standard libraries
    +---- Other libraries
    |
    v
boxes
```

---

# 9. Executable

The final executable is:

```text
build/boxes
```

Check it:

```bash
file build/boxes
```

For a native x86 build, the result would identify an x86-64 executable.

For the BeagleBone target, it should identify an ARM executable.

Example concept:

```text
ELF 32-bit
ARM
EABI
```

The important point is:

```text
Host compiler
      |
      v
ARM executable
      |
      v
BeagleBone Black
```

---

# 10. Native Build vs Cross Build

## Native Build

If you compile directly on an x86 Ubuntu machine:

```bash
g++ -std=c++17 ...
```

you normally get:

```text
x86-64 executable
```

This cannot normally run on the ARM Cortex-A8 target.

---

## Cross Compilation

For BeagleBone Black, use an ARM cross compiler.

Conceptually:

```text
arm-linux-gnueabihf-g++
```

Example:

```bash
arm-linux-gnueabihf-g++ \
    -std=c++17 \
    -Wall \
    -Wextra \
    -O2 \
    -Iapp/include \
    -c app/src/main.cpp \
    -o build/main.o
```

Then link:

```bash
arm-linux-gnueabihf-g++ \
    build/*.o \
    -o build/boxes
```

Result:

```text
build/boxes
      |
      v
ARM executable
```

---

# 11. Why Cross Compilation Is Required

The development PC may use:

```text
x86-64
```

while the BeagleBone uses:

```text
ARM Cortex-A8
```

Therefore:

```text
x86 Host
   |
   | arm-linux-gnueabihf-g++
   |
   v
ARM binary
   |
   v
BeagleBone Black
```

The compiler runs on x86 but generates ARM machine code.

---

# 12. Makefile

The Makefile automates the build.

A conceptual Makefile:

```makefile
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Iapp/include

SRC = \
    app/src/Date.cpp \
    app/src/Entry.cpp \
    app/src/StaticBox.cpp \
    app/src/DynamicBox.cpp \
    app/src/BoxManager.cpp \
    app/src/main.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = boxes

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
```

---

# 13. Build Command

From the project root:

```bash
make
```

The flow becomes:

```text
make
 |
 +--> compile Date.cpp
 |
 +--> compile Entry.cpp
 |
 +--> compile StaticBox.cpp
 |
 +--> compile DynamicBox.cpp
 |
 +--> compile BoxManager.cpp
 |
 +--> compile main.cpp
 |
 v
link
 |
 v
boxes
```

---

# 14. Clean Build

Remove previous build artifacts:

```bash
make clean
```

Then rebuild:

```bash
make
```

For a completely fresh build:

```bash
make clean
make
```

---

# 15. Unit Test Build

The project contains:

```text
app/tests/
├── test_date.cpp
├── test_dynamic_box.cpp
├── test_range_query.cpp
└── test_static_box.cpp
```

These test individual components.

Conceptually:

```text
Source
  |
  v
Build
  |
  v
Unit Tests
  |
  +---- FAIL
  |      |
  |      v
  |    Debug
  |      |
  |      v
  |    Fix
  |
  +---- PASS
         |
         v
     Cross Build
```

---

# 16. Date Test

```text
test_date.cpp
```

Tests:

```text
Date parsing
Date validation
Date comparison
Invalid dates
Ordering
```

Example:

```text
2026-08-18
```

is valid.

Invalid examples:

```text
2026-13-01
2026-02-30
invalid-date
```

---

# 17. StaticBox Test

```text
test_static_box.cpp
```

Tests:

```text
Add
Find
Capacity
Ordering
Range query
```

Conceptually:

```text
test_static_box
       |
       v
StaticBox
       |
       v
PASS / FAIL
```

---

# 18. DynamicBox Test

```text
test_dynamic_box.cpp
```

Tests:

```text
Add
Dynamic growth
Find
Ordering
Range query
```

Conceptually:

```text
test_dynamic_box
       |
       v
DynamicBox
       |
       v
std::vector
       |
       v
PASS / FAIL
```

---

# 19. Range Query Test

```text
test_range_query.cpp
```

Tests:

```text
Start date
End date
Inclusive/exclusive behavior
Empty range
Multiple records
```

Example:

```text
Range:
2026-08-10
to
2026-08-18
```

---

# 20. Cross Compilation Flow

After unit tests pass:

```text
                 Host PC
                    |
                    v
              C++ Source Code
                    |
                    v
             ARM Cross Compiler
                    |
             +------+------+
             |             |
             v             v
         Compilation     Linking
             |             |
             +------+------+
                    |
                    v
              ARM Executable
                    |
                    v
                  boxes
```

---

# 21. Application Installation

The executable is then packaged into the target root filesystem.

Target location:

```text
/opt/box-storage/bin/boxes
```

Configuration:

```text
/opt/box-storage/config/
├── boxes.conf
└── logging.conf
```

Systemd service:

```text
/etc/systemd/system/
└── box-storage.service
```

Final target filesystem:

```text
rootfs/
├── opt/
│   └── box-storage/
│       ├── bin/
│       │   └── boxes
│       │
│       └── config/
│           ├── boxes.conf
│           └── logging.conf
│
└── etc/
    └── systemd/
        └── system/
            └── box-storage.service
```

---

# 22. RootFS Creation

The root filesystem contains:

```text
Linux userspace
       |
       +--> BusyBox / core utilities
       +--> glibc
       +--> libraries
       +--> systemd
       +--> configuration
       +--> application
```

Conceptually:

```text
              RootFS
                |
     +----------+----------+
     |          |          |
     v          v          v
  Linux FS   systemd   Box Storage
                         |
                         v
                       boxes
```

---

# 23. Yocto Integration

For a production embedded Linux build, Yocto is used to generate
the complete BSP image.

Conceptually:

```text
Yocto
 |
 +--> U-Boot
 |
 +--> Linux Kernel
 |
 +--> Device Tree
 |
 +--> RootFS
 |
 +--> Box Storage Application
 |
 v
Bootable Image
```

The application should be integrated through a Yocto recipe.

Conceptually:

```text
meta-box-storage/
└── recipes-app/
    └── box-storage/
        ├── box-storage.bb
        └── files/
            ├── box-storage.service
            ├── boxes.conf
            └── logging.conf
```

---

# 24. Yocto Application Recipe

The recipe tells Yocto:

```text
Where is the source?
How should it be compiled?
Where should the binary be installed?
Which configuration files are required?
Which systemd service should be installed?
```

Conceptually:

```text
box-storage.bb
       |
       +--> Source
       |
       +--> Compile
       |
       +--> Install binary
       |
       +--> Install configuration
       |
       +--> Install systemd service
       |
       v
Yocto RootFS
```

---

# 25. Yocto Build Flow

The complete Yocto flow becomes:

```text
Source Code
     |
     v
Yocto Recipe
     |
     v
do_fetch
     |
     v
do_unpack
     |
     v
do_patch
     |
     v
do_configure
     |
     v
do_compile
     |
     v
do_install
     |
     v
RootFS
     |
     v
Image Generation
     |
     v
SD / eMMC Image
```

---

# 26. Important Yocto Tasks

### do_fetch

Obtains the source code.

```text
Source Repository
       |
       v
do_fetch
```

### do_unpack

Extracts source files.

```text
Source Archive
       |
       v
do_unpack
```

### do_patch

Applies required patches.

```text
Source
 |
 v
Patches
 |
 v
Patched Source
```

### do_configure

Prepares the build.

### do_compile

Compiles the source.

```text
.cpp
 |
 v
ARM object files
 |
 v
ARM executable
```

### do_install

Installs files into the Yocto staging/root filesystem.

```text
boxes
 |
 v
${D}/opt/box-storage/bin/boxes
```

### Image generation

Yocto combines everything:

```text
Bootloader
Kernel
Device Tree
RootFS
Application
     |
     v
Image
```

---

# 27. Image Generation

The final image may be generated in a format such as:

```text
.wic
```

Conceptually:

```text
Yocto Build
    |
    v
Bootloader
    |
    +--> MLO
    +--> U-Boot
    |
    v
Kernel
    |
    +--> Image
    +--> DTB
    |
    v
RootFS
    |
    +--> boxes
    +--> systemd
    +--> configuration
    |
    v
.wic image
```

The exact image format depends on the selected Yocto machine/image configuration.

---

# 28. SD Card Deployment

The generated image is written to the SD card.

Conceptually:

```text
.wic
 |
 v
SD Card
 |
 +--> BOOT partition
 |
 +--> ROOTFS partition
```

The project deployment script is:

```text
deployment/sd-card/deploy.sh
```

It can be responsible for preparing or deploying the generated artifacts,
depending on how the project is configured.

---

# 29. eMMC Deployment

The project also provides:

```text
deployment/emmc/
├── deploy.sh
├── install.sh
└── README.md
```

The eMMC deployment flow is:

```text
Build
  |
  v
ARM Binary
  |
  v
eMMC Deployment
  |
  v
BeagleBone eMMC
```

The exact eMMC flashing procedure depends on the selected deployment
method and boot configuration.

---

# 30. Final Build-to-Board Flow

```text
                    Developer
                        |
                        v
                  C++ Source Code
                        |
                        v
                     Makefile
                        |
                        v
                 Unit Test Build
                        |
                        v
                   Unit Tests
                        |
                  +-----+-----+
                  |           |
                FAIL         PASS
                  |           |
                  v           v
                 Fix       Cross Compile
                              |
                              v
                         ARM Binary
                              |
                              v
                       Yocto / BSP
                              |
              +---------------+---------------+
              |               |               |
              v               v               v
           U-Boot          Kernel          RootFS
                                              |
                              +---------------+
                              |
                              v
                        boxes + config
                              |
                              v
                       Image Generation
                              |
                              v
                         .wic / image
                              |
                  +-----------+-----------+
                  |                       |
                  v                       v
                SD Card                 eMMC
                  |                       |
                  +-----------+-----------+
                              |
                              v
                     BeagleBone Black
                              |
                              v
                          Boot Process
                              |
                              v
                           Linux
                              |
                              v
                           systemd
                              |
                              v
                    box-storage.service
                              |
                              v
                           boxes
```

---

# 31. Build Verification

After deployment, verify the binary:

```bash
file /opt/box-storage/bin/boxes
```

Verify permissions:

```bash
ls -l /opt/box-storage/bin/boxes
```

Verify configuration:

```bash
ls -l /opt/box-storage/config/
```

Verify systemd service:

```bash
systemctl status box-storage.service
```

---

# 32. Runtime Verification

Start the application:

```bash
sudo systemctl start box-storage.service
```

Check:

```bash
systemctl status box-storage.service
```

View logs:

```bash
journalctl -u box-storage.service
```

Test application:

```bash
/opt/box-storage/bin/boxes demo
```

---

# 33. Build Failure Debugging

Common failures:

```text
Source error
     |
     v
Compilation failure
```

Check:

```bash
make clean
make
```

---

```text
Linker error
     |
     v
Missing implementation/library
```

Check:

```text
.cpp files
libraries
linker flags
```

---

```text
Wrong architecture
     |
     v
Binary does not execute
```

Check:

```bash
file boxes
```

The binary must be built for ARM.

---

```text
Application starts manually
but systemd fails
```

Check:

```bash
systemctl status box-storage.service
journalctl -u box-storage.service
```

---

# 34. Final Verification Flow

The project is considered successfully built and deployed when:

```text
[1] Source compiles
        |
        v
[2] Unit tests PASS
        |
        v
[3] ARM binary generated
        |
        v
[4] RootFS contains boxes
        |
        v
[5] systemd service installed
        |
        v
[6] Board boots successfully
        |
        v
[7] Linux starts
        |
        v
[8] systemd starts service
        |
        v
[9] boxes starts
        |
        v
[10] Add/Get/Range operations work
```

---

# 35. One-Line Interview Explanation

The complete build flow can be explained in an interview as:

> **"We develop the C++17 Box Storage application on the host, run unit tests, cross-compile the application for the AM3358 ARM Cortex-A8, integrate the binary, configuration and systemd service into the Yocto root filesystem, generate the bootable image containing U-Boot, kernel, Device Tree and RootFS, deploy it to SD/eMMC, boot the BeagleBone, and finally validate the application through systemd and runtime tests."**

````

### Your `docs/` is now logically organized

```text
docs/
├── architecture.md    → How the software is designed
├── boot_flow.md       → Power ON → ROM → SPL → U-Boot → Linux → app
└── build_flow.md      → Source → compile → link → Yocto → image → board
````

These three documents together give you a very strong **embedded-Linux project story**: **Architecture → Build → Boot → Runtime**.

