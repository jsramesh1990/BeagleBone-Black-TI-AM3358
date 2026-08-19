
# BeagleBone Black – TI AM3358

## Date-Indexed Box Storage System

> Embedded Linux C++ storage application deployed on the BeagleBone Black using the TI AM3358 processor, with CMake/Make-based development, Yocto integration, systemd service management, SD-card deployment, and automated testing.

---

## Complete BeagleBone Black System Flow

The following animation illustrates the complete BeagleBone Black based on the TI AM3358 execution flow, from power-on, ROM boot, SPL and U-Boot through the Linux kernel, Device Tree, kernel drivers, userspace, and application-level peripheral access.

<p align="center">
  <img src="images/BBB-TI%20AM3358.gif"
       alt="BeagleBone Black TI AM3358 Complete System Flow"
       width="950">
</p>


## 1. Project Overview

The **BeagleBone Black – TI AM3358 Box Storage System** is an embedded Linux application designed to store and retrieve timestamped data efficiently.

The application provides two storage implementations:

* `StaticBox<N>` – fixed-capacity storage using static allocation.
* `DynamicBox` – dynamically growing storage using `std::vector`.

Data is indexed using a `Date` structure and supports chronological searching and range queries.

The project demonstrates a complete embedded software development flow:

```text
C++ Application
      |
      v
CMake
      |
      v
Makefile
      |
      v
Unit Testing
      |
      v
Cross Compilation
      |
      v
Yocto Recipe
      |
      v
Linux RootFS
      |
      v
Bootable Image
      |
      v
SD Card
      |
      v
BeagleBone Black
      |
      v
U-Boot
      |
      v
Linux Kernel
      |
      v
systemd
      |
      v
Box Storage Application
```

---

# 2. Hardware Platform

## BeagleBone Black

The target platform is the **BeagleBone Black**, based on the **Texas Instruments AM3358** SoC.

### Processor

| Feature           | Specification     |
| ----------------- | ----------------- |
| SoC               | TI AM3358         |
| CPU               | ARM Cortex-A8     |
| CPU cores         | 1                 |
| Architecture      | ARMv7-A           |
| Typical frequency | Up to 1 GHz       |
| Floating point    | NEON/VFP          |
| Target OS         | Embedded Linux    |
| Boot media        | microSD / eMMC    |
| Ethernet          | 10/100 Ethernet   |
| USB               | USB Host / Device |
| GPIO              | Multiple GPIOs    |
| Serial            | UART              |
| I²C               | Available         |
| SPI               | Available         |
| PWM               | Available         |
| ADC               | Available         |

### Simplified hardware architecture

```text
                     BeagleBone Black
                            |
                      TI AM3358 SoC
                            |
             +--------------+--------------+
             |              |              |
             v              v              v
          Cortex-A8       DDR3L          Peripherals
             |              |              |
             |              |       +------+------+
             |              |       |      |      |
             v              |      UART   I2C    SPI
       Embedded Linux       |       |      |      |
             |              |       +------+------+
             |              |
             v              v
          RootFS          Memory
             |
             v
       Box Storage App
```

---

# 3. Why BeagleBone Black?

The BeagleBone Black is suitable for this project because it provides:

* ARM Cortex-A8 processing
* Embedded Linux support
* SD-card boot
* eMMC storage
* GPIO
* UART
* I²C
* SPI
* PWM
* Ethernet
* USB
* Large open-source community
* Yocto support
* U-Boot support

It is therefore suitable for demonstrating a complete **embedded Linux BSP + application deployment flow**.

---

# 4. Project Objectives

The main objectives are:

1. Develop a date-indexed C++ storage application.
2. Implement static and dynamic storage models.
3. Support date validation and comparison.
4. Implement chronological data retrieval.
5. Implement range queries.
6. Provide unit testing.
7. Support CMake-based builds.
8. Provide Makefile automation.
9. Support ARM cross compilation.
10. Integrate the application into Yocto.
11. Create a Linux RootFS containing the application.
12. Start the application through systemd.
13. Deploy the image to SD card.
14. Boot the BeagleBone Black.
15. Validate application behavior on the target.

---

# 5. Repository Structure

```text
BeagleBone_Black-TI_AM3358/
│
├── README.md
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
│   │   ├── deploy.sh
│   │   ├── install.sh
│   │   └── README.md
│   │
│   ├── sd-card/
│   │   ├── boot/
│   │   ├── rootfs/
│   │   ├── deploy.sh
│   │   └── README.md
│   │
│   └── systemd/
│       └── box-storage.service
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
        ├── conf/
        │   └── layer.conf
        │
        ├── recipes-box/
        │   └── box-storage/
        │       ├── box-storage.bb
        │       └── files/
        │           ├── box-storage.service
        │           ├── boxes.conf
        │           └── logging.conf
        │
        └── README.md
```

---

# 6. Application Architecture

The application is divided into multiple C++ components.

```text
                         main.cpp
                            |
                            v
                      BoxManager
                            |
              +-------------+-------------+
              |                           |
              v                           v
         StaticBox<N>                DynamicBox
              |                           |
              +-------------+-------------+
                            |
                            v
                          Entry
                            |
                            v
                           Date
```

---

# 7. `Date`

Location:

```text
app/include/Date.hpp
app/src/Date.cpp
```

The `Date` class represents:

```text
YYYY-MM-DD
```

Example:

```cpp
Date date(2026, 8, 18);
```

or:

```cpp
Date::from_string("2026-08-18");
```

Responsibilities:

* Date parsing
* Date validation
* Date comparison
* Chronological ordering

Example:

```text
2026-01-01 < 2026-06-01 < 2026-08-18
```

---

# 8. `Entry`

Location:

```text
app/include/Entry.hpp
app/src/Entry.cpp
```

An `Entry` combines:

```text
Date + Payload
```

Example:

```text
Date:    2026-08-18
Payload: "Sensor data"
```

Conceptually:

```cpp
struct Entry
{
    Date date;
    std::string payload;
};
```

---

# 9. `StaticBox`

Location:

```text
app/include/StaticBox.hpp
app/src/StaticBox.cpp
```

`StaticBox<N>` provides fixed-capacity storage.

Example:

```cpp
StaticBox<50> box;
```

Conceptually:

```text
Stack / object memory
        |
        v
+----------------------+
| Entry 0              |
| Entry 1              |
| Entry 2              |
| ...                  |
| Entry 49             |
+----------------------+
```

Characteristics:

* Fixed capacity
* No dynamic resizing
* Predictable memory usage
* Good cache locality
* Suitable for constrained embedded systems

---

# 10. `DynamicBox`

Location:

```text
app/include/DynamicBox.hpp
app/src/DynamicBox.cpp
```

Uses dynamically allocated storage.

Conceptually:

```text
Dynamic memory
      |
      v
std::vector<Entry>
      |
      +---- Entry
      +---- Entry
      +---- Entry
      +---- ...
```

Advantages:

* Runtime resizing
* No fixed capacity
* Flexible storage

Disadvantage:

* Heap allocation
* Possible reallocation
* Less deterministic memory behavior

---

# 11. `BoxManager`

Location:

```text
app/include/BoxManager.hpp
app/src/BoxManager.cpp
```

`BoxManager` provides the higher-level interface between the application and storage containers.

```text
             Application
                  |
                  v
             BoxManager
              /       \
             /         \
            v           v
       StaticBox    DynamicBox
```

It handles operations such as:

```text
ADD
GET
RANGE QUERY
```

---

# 12. Storage Flow

Example:

```bash
./boxes add dynamic 2026-08-18 "Sensor data"
```

Flow:

```text
Command line
     |
     v
main.cpp
     |
     v
BoxManager
     |
     v
DynamicBox
     |
     v
Date + Payload
     |
     v
Stored Entry
```

---

# 13. Date Range Query

Example:

```bash
./boxes range dynamic 2026-08-01 2026-08-18
```

Flow:

```text
Start Date
2026-08-01
      |
      v
+-----------------------------+
| DynamicBox                  |
|                             |
| 08-01                       |
| 08-05                       |
| 08-10                       |
| 08-15                       |
| 08-18                       |
| 08-20                       |
+-----------------------------+
      |
      v
End Date
2026-08-18
      |
      v
Matching Entries
```

If entries are sorted by date, binary search can be used to locate the relevant range efficiently.

---

# 14. Time Complexity

| Operation   |       StaticBox |                    DynamicBox |
| ----------- | --------------: | ----------------------------: |
| Add         | O(n) worst-case |               O(n) worst-case |
| Search      |        O(log n) |                      O(log n) |
| Range query |    O(log n + k) |                  O(log n + k) |
| Comparison  |            O(1) |                          O(1) |
| Resize      |  Not applicable | O(n) when reallocation occurs |

Where `k` is the number of returned entries.

---

# 15. Build System

The project supports:

```text
CMake
Make
Shell scripts
Yocto
```

The relationship is:

```text
                    Makefile
                       |
                       v
                    CMake
                       |
                       v
              C++ Compilation
                       |
             +---------+---------+
             |                   |
             v                   v
         Host Build          ARM Build
```

---

# 16. Prerequisites

Host machine:

```bash
sudo apt update
sudo apt install build-essential cmake git
```

Check:

```bash
g++ --version
cmake --version
make --version
```

Expected:

```text
g++      C++17 capable
CMake    >= 3.16
Make     available
```

---

# 17. Build Using Make

From project root:

```bash
make
```

or:

```bash
make build
```

Flow:

```text
make
 |
 v
CMake
 |
 v
Compiler
 |
 v
C++ object files
 |
 v
box_storage_lib
 |
 v
boxes
```

---

# 18. Build Using CMake Directly

```bash
mkdir -p build

cmake \
    -S . \
    -B build \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build --parallel
```

Binary:

```text
build/boxes
```

---

# 19. Debug Build

```bash
cmake \
    -S . \
    -B build \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build build
```

---

# 20. Run Application

```bash
./build/boxes
```

or:

```bash
make run
```

Example:

```bash
make demo
```

---

# 21. Unit Testing

Tests are located in:

```text
app/tests/
```

Run:

```bash
make test
```

or:

```bash
cd build
ctest --output-on-failure
```

Expected:

```text
Test #1: DateTest
Test #2: StaticBoxTest
Test #3: DynamicBoxTest
Test #4: RangeQueryTest

100% tests passed
```

---

# 22. Test Architecture

```text
                 CMake
                   |
                   v
             Test Executables
                   |
       +-----------+-----------+
       |           |           |
       v           v           v
   Date Test   Static Test  Dynamic Test
       |           |           |
       +-----------+-----------+
                   |
                   v
              Range Test
```

---

# 23. Output Directory

Build artifacts are placed under:

```text
output/
├── host/
│   ├── bin/
│   ├── obj/
│   └── tests/
│
├── target/
│   ├── bin/
│   ├── config/
│   └── systemd/
│
└── logs/
    ├── build/
    ├── test/
    ├── deployment/
    ├── boot/
    └── runtime/
```

### Host

```text
output/host/
```

contains binaries intended for the development machine.

### Target

```text
output/target/
```

contains files intended for the BeagleBone Linux RootFS.

### Logs

```text
output/logs/
```

contains collected build/test/deployment/runtime information.

---

# 24. Clean

Normal clean:

```bash
make clean
```

Complete clean:

```bash
make distclean
```

or:

```bash
./scripts/clean.sh
```

---

# 25. Cross Compilation

The BeagleBone Black uses an ARM Cortex-A8.

The development PC may be x86-64.

Therefore:

```text
Development PC
x86-64
   |
   | Cross compiler
   v
ARM executable
   |
   v
BeagleBone Black
ARM Cortex-A8
```

For example:

```bash
arm-linux-gnueabihf-g++
```

The exact toolchain should match the target Linux userspace/ABI.

---

# 26. Yocto Integration

The project contains a custom Yocto layer:

```text
yocto/meta-box-storage/
```

Structure:

```text
meta-box-storage/
├── conf/
│   └── layer.conf
│
├── recipes-box/
│   └── box-storage/
│       ├── box-storage.bb
│       └── files/
│           ├── box-storage.service
│           ├── boxes.conf
│           └── logging.conf
│
└── README.md
```

---

# 27. Add Yocto Layer

From the Yocto build environment:

```bash
bitbake-layers add-layer \
    ../meta-box-storage
```

Verify:

```bash
bitbake-layers show-layers
```

Expected:

```text
meta-box-storage
```

---

# 28. Build Yocto Recipe

```bash
bitbake box-storage
```

Flow:

```text
box-storage.bb
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
do_package
```

---

# 29. Add Application to Image

In the Yocto configuration:

```bitbake
IMAGE_INSTALL:append = " box-storage"
```

Then build the image:

```bash
bitbake <image-name>
```

For example:

```bash
bitbake core-image-minimal
```

The exact image name depends on the BSP configuration.

---

# 30. Yocto Installation Result

After image creation, the target RootFS contains:

```text
/usr/bin/boxes

/etc/box-storage/
├── boxes.conf
└── logging.conf

/etc/systemd/system/
└── box-storage.service
```

---

# 31. systemd Service

Service:

```text
deployment/systemd/box-storage.service
```

or the Yocto recipe's copy under:

```text
yocto/meta-box-storage/recipes-box/box-storage/files/
```

The service starts:

```text
/usr/bin/boxes
```

Boot sequence:

```text
Linux boot
    |
    v
systemd
    |
    v
box-storage.service
    |
    v
/usr/bin/boxes
```

Check on target:

```bash
systemctl status box-storage.service
```

Start manually:

```bash
systemctl start box-storage.service
```

Enable at boot:

```bash
systemctl enable box-storage.service
```

---

# 32. SD Card Deployment

Deployment consists of two stages.

### Stage 1 — Prepare files

```bash
./scripts/deploy.sh
```

Flow:

```text
output/target/
      |
      v
deployment/sd-card/rootfs/
```

### Stage 2 — Flash physical SD card

First identify the SD card:

```bash
lsblk
```

Then use the project flashing script:

```bash
sudo ./scripts/flash_sd.sh /dev/sdX
```

**Always verify `/dev/sdX` before flashing. Selecting the wrong device can destroy data on the host machine.**

---

# 33. SD Card Layout

The SD card normally contains at least:

```text
SD Card
│
├── Boot Partition
│   ├── MLO / SPL
│   ├── u-boot.img
│   ├── Image
│   └── *.dtb
│
└── RootFS Partition
    ├── bin/
    ├── etc/
    ├── lib/
    ├── usr/
    │   └── bin/
    │       └── boxes
    └── var/
```

The exact boot filenames and partition layout depend on the selected BeagleBone/Yocto BSP.

---

# 34. BeagleBone Black Boot Flow

The complete boot flow is:

```text
                    POWER ON
                       |
                       v
                AM3358 Boot ROM
                       |
                       v
              Boot Device Detection
                       |
              +--------+--------+
              |                 |
              v                 v
             SD               eMMC
              |                 |
              +--------+--------+
                       |
                       v
                    SPL/MLO
                       |
                       v
                    DDR Init
                       |
                       v
                    U-Boot
                       |
             +---------+---------+
             |                   |
             v                   v
        Load Kernel          Load DTB
             |                   |
             +---------+---------+
                       |
                       v
                 Linux Kernel
                       |
                       v
                  Device Tree
                       |
                       v
                    RootFS
                       |
                       v
                   systemd
                       |
                       v
             box-storage.service
                       |
                       v
                  boxes app
```

---

# 35. Boot Stage Responsibilities

## Boot ROM

Implemented inside the AM3358.

Responsible for:

* Initial processor startup
* Boot-device selection
* Loading the first bootloader stage

---

## SPL / MLO

Initial bootloader stage.

Responsibilities include:

* Basic CPU initialization
* Clock setup
* DDR initialization
* Loading U-Boot

---

## U-Boot

Responsibilities:

* Hardware initialization
* Environment loading
* Boot arguments
* Loading Linux kernel
* Loading Device Tree
* Passing control to Linux

Typical boot arguments include:

```text
console=
root=
rootwait
rw
```

---

## Linux Kernel

Initializes:

* CPU
* Memory
* Interrupt controller
* Timers
* Drivers
* Filesystems
* Network
* Storage
* Other peripherals

Then mounts the RootFS.

---

## systemd

Once Linux userspace starts:

```text
systemd
   |
   +-- services
   +-- networking
   +-- logging
   +-- application
```

Our application is started through:

```text
box-storage.service
```

---

# 36. Complete Boot-to-Application Flow

```text
Power ON
   |
   v
AM3358
Boot ROM
   |
   v
SPL/MLO
   |
   v
DDR initialization
   |
   v
U-Boot
   |
   v
Read boot configuration
   |
   v
Load Kernel
   |
   v
Load Device Tree
   |
   v
Kernel startup
   |
   v
Driver initialization
   |
   v
RootFS mount
   |
   v
systemd
   |
   v
box-storage.service
   |
   v
/usr/bin/boxes
   |
   v
Box Storage System
```

---

# 37. Runtime Validation

After booting the BeagleBone:

Check kernel:

```bash
uname -a
```

Check CPU:

```bash
cat /proc/cpuinfo
```

Check memory:

```bash
free -h
```

Check storage:

```bash
df -h
```

Check application:

```bash
which boxes
```

Expected:

```text
/usr/bin/boxes
```

---

# 38. Service Validation

```bash
systemctl status box-storage.service
```

Check logs:

```bash
journalctl -u box-storage.service
```

Follow logs:

```bash
journalctl -u box-storage.service -f
```

Check kernel messages:

```bash
dmesg
```

---

# 39. Application Validation

Example:

```bash
boxes add static 2026-08-18 "Sensor data"
```

Retrieve:

```bash
boxes get static 2026-08-18
```

Range query:

```bash
boxes range dynamic 2026-08-01 2026-08-18
```

Expected behavior:

```text
ADD
 |
 v
Date validation
 |
 v
Entry creation
 |
 v
BoxManager
 |
 v
StaticBox/DynamicBox
 |
 v
Sorted storage
 |
 v
GET/RANGE
 |
 v
Output
```

---

# 40. Debugging

## Application not found

```bash
which boxes
```

If missing:

```bash
ls -l /usr/bin/boxes
```

---

## Service not running

```bash
systemctl status box-storage.service
```

Then:

```bash
journalctl -u box-storage.service
```

---

## Kernel boot problem

Check serial console and:

```bash
dmesg
```

Check:

```bash
cat /proc/cmdline
```

---

## RootFS problem

Check:

```bash
mount
df -h
```

---

## Permission problem

```bash
ls -l /usr/bin/boxes
```

Expected executable permission:

```text
-rwxr-xr-x
```

---

# 41. Development Workflow

Recommended daily workflow:

```text
1. Modify C++ source
        |
        v
2. make build
        |
        v
3. make test
        |
        v
4. Fix failures
        |
        v
5. make run
        |
        v
6. Validate behavior
        |
        v
7. Build ARM/Yocto target
        |
        v
8. Deploy
        |
        v
9. Boot BeagleBone
        |
        v
10. Validate service
        |
        v
11. Validate application
```

---

# 42. Production Image Workflow

For the final embedded target:

```text
              C++ Source
                  |
                  v
          meta-box-storage
                  |
                  v
             BitBake
                  |
                  v
          Cross Compilation
                  |
                  v
             Packaging
                  |
                  v
              RootFS
                  |
                  v
           Linux Image
                  |
                  v
              SD Image
                  |
                  v
          BeagleBone Black
                  |
                  v
              U-Boot
                  |
                  v
          Linux Kernel
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

# 43. Build Commands – Quick Reference

### Build

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

### Deploy

```bash
make deploy
```

### Yocto recipe

```bash
make yocto
```

### Yocto image

```bash
make image
```

### Identify SD card

```bash
lsblk
```

### Flash

```bash
sudo ./scripts/flash_sd.sh /dev/sdX
```

---

# 44. Evaluation Strategy

The project is evaluated at four levels.

## Level 1 – C++ correctness

Verify:

* Date parsing
* Date comparison
* Entry creation
* StaticBox
* DynamicBox
* Add
* Get
* Range query

---

## Level 2 – Unit tests

```bash
make test
```

All tests must pass.

---

## Level 3 – Embedded deployment

Verify:

```text
Yocto build
    ↓
Image creation
    ↓
SD deployment
    ↓
BeagleBone boot
```

---

## Level 4 – Runtime

Verify:

```text
systemd
   ↓
box-storage.service
   ↓
/usr/bin/boxes
   ↓
Application functionality
```

---

# 45. Performance Evaluation

The project can be evaluated using:

* Application startup time
* Memory consumption
* Add operation latency
* Search latency
* Range-query latency
* Static vs dynamic storage performance
* CPU usage
* Storage usage

Useful commands:

```bash
time boxes
```

Memory:

```bash
/usr/bin/time -v boxes
```

CPU/process:

```bash
top
```

or:

```bash
ps aux | grep boxes
```

---

# 46. Memory Evaluation

Static storage:

```text
StaticBox<N>
      |
      v
Fixed memory
      |
      v
Predictable usage
```

Dynamic storage:

```text
DynamicBox
      |
      v
std::vector
      |
      v
Heap
      |
      v
Possible reallocation
```

This comparison is especially relevant for embedded systems.

---

# 47. Security and Reliability Considerations

The application should consider:

* Input validation
* Date validation
* Invalid command handling
* File permissions
* systemd restart policy
* Resource limits
* Memory usage
* Logging
* Error handling

The application should not crash on invalid input.

---

# 48. Important Design Principle

The project separates:

```text
Application
     |
     +---- C++
     |
Build
     |
     +---- CMake
     |
Automation
     |
     +---- Makefile/scripts
     |
Embedded Integration
     |
     +---- Yocto
     |
Deployment
     |
     +---- SD/eMMC
     |
Runtime
     |
     +---- systemd
```

This separation makes the project maintainable.

---

# 49. Complete Project Flow

The complete system can be summarized as:

```text
                         DEVELOPMENT PC
                              |
                              |
                       C++ Source Code
                              |
                              v
                    +-------------------+
                    |     CMake         |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    |     Makefile      |
                    +---------+---------+
                              |
                              v
                       Build + Tests
                              |
                              v
                    +-------------------+
                    |     output/       |
                    +---------+---------+
                              |
                              v
                       ARM Target Build
                              |
                              v
                    +-------------------+
                    |  Yocto Layer      |
                    | meta-box-storage  |
                    +---------+---------+
                              |
                              v
                         BitBake
                              |
                              v
                    +-------------------+
                    | Linux RootFS      |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    | Kernel + DTB      |
                    +---------+---------+
                              |
                              v
                         SD Image
                              |
                              v
                    +-------------------+
                    | BeagleBone Black  |
                    | TI AM3358         |
                    +---------+---------+
                              |
                              v
                         Boot ROM
                              |
                              v
                           SPL/MLO
                              |
                              v
                           U-Boot
                              |
                              v
                        Linux Kernel
                              |
                              v
                            RootFS
                              |
                              v
                           systemd
                              |
                              v
                    box-storage.service
                              |
                              v
                       /usr/bin/boxes
                              |
                              v
                    Date Indexed Storage
                              |
                              v
                       FINAL OUTPUT
```

---

# 50. Interview Explanation

A concise way to explain the entire project is:

> **"I developed a date-indexed C++ storage application for the BeagleBone Black based on the TI AM3358 Cortex-A8. The application provides static and dynamic storage containers with date validation, sorted storage and range queries. I structured the application using CMake and Makefile automation, added unit tests, and integrated the application into a custom Yocto layer. BitBake cross-compiles and packages the application into the target RootFS along with its configuration and systemd service. The generated Linux image is deployed to an SD card, from which the AM3358 Boot ROM starts SPL and U-Boot, U-Boot loads the Linux kernel and Device Tree, and systemd starts the Box Storage service. I validate the system at application, unit-test, boot, service and runtime levels."**

---

## 51. Recommended Documentation Relationship

Your individual documentation files should now have these responsibilities:

```text
README.md
    |
    +-- Overall project
    +-- Hardware
    +-- Build
    +-- Flash
    +-- Boot
    +-- Test
    +-- Runtime
         |
         +-----------------------------+
                                       |
       +---------------+---------------+---------------+
       |               |               |               |
       v               v               v               v
architecture.md   build_flow.md   boot_flow.md   storage_flow.md
                                      |
                              +-------+-------+
                              |               |
                              v               v
                         debugging.md    testing.md
```

So the **README is your master document**, while the `docs/*.md` files contain the deep technical explanations.

For your project specifically, the most important next document to make consistent with this README is **`docs/build_flow.md`**, because that should show the exact chain from **`make build` → CMake → compiler → object files → executable → ARM cross-compilation → Yocto/BitBake → RootFS → final image**.
