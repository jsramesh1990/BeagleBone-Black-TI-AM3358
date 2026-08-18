Yes. Add `debugging.md` under `docs/`. For this project, it should cover **debugging from C++ source → binary → Linux → systemd → BeagleBone hardware → runtime output**.

Your structure becomes:

```text
BeagleBone_Black-TI_AM3358/
└── docs/
    ├── architecture.md
    ├── boot_flow.md
    ├── build_flow.md
    └── debugging.md
```

# `docs/debugging.md`

````markdown
# BeagleBone Black TI AM3358 Debugging Guide

## 1. Overview

This document describes the debugging methodology for the
Box Storage System running on the BeagleBone Black using the
TI AM3358 processor.

Debugging is performed at multiple levels:

```text
Application
     |
     v
C++ Runtime
     |
     v
Systemd
     |
     v
Linux Kernel
     |
     v
Device Tree / Drivers
     |
     v
Bootloader
     |
     v
Hardware
````

The goal is to identify exactly which layer is causing a failure.

---

# 2. Debugging Architecture

The complete debugging flow is:

```text
                  Problem
                     |
                     v
              Identify Layer
                     |
       +-------------+-------------+
       |             |             |
       v             v             v
    Build        Boot/OS       Application
       |             |             |
       v             v             v
   Compiler       U-Boot       systemd
   Linker         Kernel       logs
   Binary         Driver       GDB
                     |
                     v
                  Hardware
```

---

# 3. Debugging Layers

## Layer 1 - Source Code

Files:

```text
app/src/
├── Date.cpp
├── Entry.cpp
├── StaticBox.cpp
├── DynamicBox.cpp
├── BoxManager.cpp
└── main.cpp
```

Typical problems:

```text
Syntax errors
Logic errors
Invalid date handling
Memory errors
Incorrect search
Incorrect range query
```

---

## Layer 2 - Build System

Files:

```text
Makefile
```

Typical problems:

```text
Compilation failure
Missing header
Linker failure
Wrong compiler
Wrong compiler flags
Wrong architecture
Missing library
```

---

## Layer 3 - Target Binary

Target:

```text
/opt/box-storage/bin/boxes
```

Typical problems:

```text
Binary does not execute
Wrong architecture
Missing shared library
Permission denied
Missing dynamic linker
```

---

## Layer 4 - RootFS

Typical problems:

```text
Missing binary
Missing configuration
Missing library
Incorrect permissions
Incorrect filesystem path
```

---

## Layer 5 - systemd

Service:

```text
/etc/systemd/system/box-storage.service
```

Typical problems:

```text
Service fails to start
Incorrect ExecStart
Missing dependency
Incorrect working directory
Permission problem
Configuration problem
```

---

## Layer 6 - Linux Kernel

Typical problems:

```text
Driver failure
Device Tree error
Storage failure
MMC failure
USB failure
Ethernet failure
Memory problem
```

---

## Layer 7 - Bootloader

Typical problems:

```text
U-Boot does not start
Kernel not found
Device Tree not found
Incorrect boot arguments
Incorrect boot target
```

---

# 4. First Rule of Debugging

Always determine:

```text
What works?
What does not work?
Where does the failure begin?
```

Do not immediately change source code.

Use the following flow:

```text
Power ON
   |
   ? Boot ROM
   |
   ? SPL
   |
   ? U-Boot
   |
   ? Linux Kernel
   |
   ? RootFS
   |
   ? systemd
   |
   ? Application
   |
   v
Output
```

Find the first failed stage.

---

# 5. Build Debugging

## Clean Build

Always start with:

```bash
make clean
make
```

If the build fails, identify whether the error is:

```text
Compiler
Linker
Makefile
Header
Library
```

---

# 6. Compilation Error

Example:

```text
fatal error: Date.hpp: No such file or directory
```

Check:

```bash
ls app/include/Date.hpp
```

Check include path:

```bash
-Iapp/include
```

Build example:

```bash
g++ -std=c++17 \
    -Iapp/include \
    -c app/src/Date.cpp
```

---

# 7. Linker Error

Example:

```text
undefined reference to `Date::...`
```

This usually means the declaration exists but the
implementation object file was not linked.

Check:

```text
Date.cpp
Date.o
```

Make sure it participates in the final link:

```text
Date.o
Entry.o
StaticBox.o
DynamicBox.o
BoxManager.o
main.o
```

---

# 8. Wrong Architecture

Check the binary:

```bash
file boxes
```

For BeagleBone Black, the binary must target ARM.

Conceptually:

```text
Correct:

ELF 32-bit
ARM
EABI

Incorrect:

ELF 64-bit
x86-64
```

If the wrong architecture was generated:

```text
Host compiler
      |
      v
Wrong binary
      |
      X
BeagleBone
```

Use the appropriate ARM cross compiler.

---

# 9. Debug Build

For debugging, don't optimize aggressively.

Use:

```text
-g
```

Example:

```bash
g++ -std=c++17 \
    -g \
    -O0 \
    -Wall \
    -Wextra \
    -Iapp/include \
    ...
```

Meaning:

```text
-g  -> debugging symbols
-O0 -> disable optimization
```

This makes GDB debugging easier.

---

# 10. AddressSanitizer

For host-side testing, AddressSanitizer can detect memory errors.

Example:

```bash
g++ -std=c++17 \
    -g \
    -O0 \
    -fsanitize=address \
    -Iapp/include \
    ...
```

It can help detect:

```text
Buffer overflow
Use-after-free
Stack corruption
Invalid memory access
```

Run:

```bash
./boxes
```

---

# 11. Valgrind

Valgrind can also be used for host-side testing.

Example:

```bash
valgrind ./boxes demo
```

It can detect:

```text
Memory leaks
Invalid reads
Invalid writes
Invalid frees
```

Important:

```text
Host debugging
     |
     v
Valgrind
```

is generally easier than trying to run Valgrind directly on a
small embedded target.

---

# 12. Application Logging

The application should provide useful logs.

Example:

```text
INFO: Application starting
INFO: Loading configuration
INFO: Creating DynamicBox
INFO: Adding entry
INFO: Date = 2026-08-18
INFO: Application ready
```

Avoid debugging using only:

```cpp
std::cout
```

for a systemd-managed embedded application.

Prefer a logging mechanism that integrates with the target system.

---

# 13. Manual Application Test

First run the application manually.

```bash
/opt/box-storage/bin/boxes demo
```

If this works:

```text
Application
     |
     v
works manually
```

but systemd fails:

```text
systemd
     |
     X
service problem
```

This immediately narrows the problem.

---

# 14. Check Application Permissions

Check:

```bash
ls -l /opt/box-storage/bin/boxes
```

Expected conceptually:

```text
-rwxr-xr-x
```

If required:

```bash
chmod +x /opt/box-storage/bin/boxes
```

Then:

```bash
/opt/box-storage/bin/boxes demo
```

---

# 15. Check Shared Libraries

Use:

```bash
ldd /opt/box-storage/bin/boxes
```

Look for:

```text
not found
```

Example:

```text
libsomething.so => not found
```

This indicates a missing runtime dependency.

---

# 16. Systemd Debugging

Check service status:

```bash
systemctl status box-storage.service
```

Example:

```text
● box-storage.service
   Loaded: loaded
   Active: active (running)
```

If:

```text
Active: failed
```

investigate the service.

---

# 17. Systemd Logs

Use:

```bash
journalctl -u box-storage.service
```

For the latest boot:

```bash
journalctl -u box-storage.service -b
```

Follow logs live:

```bash
journalctl -u box-storage.service -f
```

This is one of the most important commands for this project.

---

# 18. Restart Service

After modifying the service:

```bash
systemctl daemon-reload
```

Then:

```bash
systemctl restart box-storage.service
```

Check:

```bash
systemctl status box-storage.service
```

---

# 19. Check Service File

Service location:

```text
/etc/systemd/system/box-storage.service
```

Check:

```bash
cat /etc/systemd/system/box-storage.service
```

Important fields:

```text
[Unit]
[Service]
[Install]
```

Especially:

```text
ExecStart=
WorkingDirectory=
User=
Restart=
```

---

# 20. Typical Service Failure

Example:

```text
ExecStart=/opt/box-storage/bin/boxes
```

If the executable doesn't exist:

```text
systemd
   |
   v
ExecStart
   |
   X
File not found
```

Check:

```bash
ls -l /opt/box-storage/bin/boxes
```

---

# 21. Kernel Debugging

If the application cannot access a hardware device,
inspect the kernel first.

Use:

```bash
dmesg
```

For recent messages:

```bash
dmesg | tail -100
```

Search:

```bash
dmesg | grep -i error
```

or:

```bash
dmesg | grep -i fail
```

---

# 22. Linux Kernel Boot Logs

During boot:

```text
U-Boot
   |
   v
Linux Kernel
   |
   +--> CPU initialization
   +--> Memory
   +--> Drivers
   +--> Filesystem
   |
   v
userspace
```

If the kernel hangs before userspace:

```text
Application is NOT the problem.
```

The investigation should move to:

```text
Kernel
Device Tree
Boot arguments
Drivers
Hardware
```

---

# 23. Device Tree Debugging

Device Tree problems can prevent peripherals from working.

Check the live Device Tree:

```bash
ls /proc/device-tree
```

Depending on the kernel:

```bash
find /proc/device-tree -maxdepth 2
```

Look for expected peripherals.

Conceptually:

```text
Device Tree
    |
    +--> UART
    +--> I2C
    +--> SPI
    +--> GPIO
    +--> MMC
    +--> USB
```

If a peripheral is missing or disabled:

```text
Device Tree
     |
     X
Driver/device not initialized
```

---

# 24. Driver Debugging

Check kernel messages:

```bash
dmesg | grep -i <device>
```

Check loaded modules:

```bash
lsmod
```

Check devices:

```bash
ls /dev
```

For example:

```text
/dev/tty*
/dev/mmc*
/dev/i2c*
```

The exact device nodes depend on the enabled hardware and drivers.

---

# 25. Storage Debugging

Check block devices:

```bash
lsblk
```

Example concept:

```text
mmcblk0
├── mmcblk0p1
└── mmcblk0p2
```

Typically:

```text
p1 -> boot
p2 -> root filesystem
```

Check mounts:

```bash
mount
```

or:

```bash
findmnt
```

---

# 26. RootFS Debugging

Check:

```bash
df -h
```

This tells you filesystem usage.

Check application:

```bash
ls -lah /opt/box-storage/
```

Expected:

```text
/opt/box-storage/
├── bin/
│   └── boxes
└── config/
    ├── boxes.conf
    └── logging.conf
```

---

# 27. Bootloader Debugging

If the board does not reach Linux:

```text
Power ON
   |
   v
Boot ROM
   |
   X
SPL
```

or:

```text
SPL
 |
 v
U-Boot
 |
 X
```

Use the serial console to observe the boot messages.

---

# 28. U-Boot Commands

At the U-Boot prompt, useful commands include:

```bash
printenv
```

View environment variables.

```bash
printenv bootcmd
```

View boot command.

```bash
printenv bootargs
```

View kernel boot arguments.

```bash
mmc list
```

List MMC devices.

```bash
mmc dev
```

Select an MMC device.

The exact available commands depend on the U-Boot version/configuration.

---

# 29. Kernel Not Found

Typical problem:

```text
U-Boot
   |
   +--> Search kernel
   |
   X
Kernel not found
```

Check:

```text
BOOT partition
├── Image
└── Device Tree
```

Verify the files exist on the boot media.

---

# 30. RootFS Not Found

Typical problem:

```text
Kernel
  |
  v
Mount root filesystem
  |
  X
```

Check:

```text
bootargs
root device
filesystem
storage
partition
```

The root filesystem location must match the actual storage
configuration.

---

# 31. Serial Console

Serial console is one of the most important debugging tools
for the BeagleBone.

It allows observation of:

```text
SPL
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
Application
```

Without serial output, early boot failures are much harder
to diagnose.

---

# 32. GDB Debugging

For application-level debugging, compile with:

```bash
-g
```

Example:

```bash
g++ -g -O0 ...
```

Then use GDB.

Host-side:

```bash
gdb ./boxes
```

Useful commands:

```text
break main
run
next
step
continue
print
backtrace
info locals
```

---

# 33. GDB Debugging Flow

```text
boxes
  |
  v
GDB
  |
  v
main()
  |
  v
BoxManager
  |
  v
StaticBox / DynamicBox
  |
  v
Entry
  |
  v
Date
```

Example:

```text
break main
run
```

Then:

```text
next
step
print variable
```

---

# 34. Remote GDB

For target debugging:

```text
Host PC
   |
   | GDB
   |
   v
BeagleBone
   |
   v
gdbserver
   |
   v
boxes
```

Conceptually:

```text
                 Host PC
                    |
                    |
                 GDB Client
                    |
                 Network
                    |
                    v
              BeagleBone
                    |
                gdbserver
                    |
                    v
                  boxes
```

This allows debugging the actual ARM executable on the
BeagleBone while controlling it from the host PC.

---

# 35. Application Crash

If the application crashes:

```text
boxes
 |
 X
Segmentation fault
```

First collect:

```bash
dmesg | tail
```

and:

```bash
journalctl -u box-storage.service
```

If a core dump is enabled:

```bash
coredumpctl
```

Then inspect with GDB.

---

# 36. Segmentation Fault Debugging

Typical causes:

```text
Null pointer
Invalid pointer
Out-of-bounds access
Use-after-free
Stack corruption
Invalid object lifetime
```

Debugging flow:

```text
Segmentation Fault
       |
       v
Get crash information
       |
       v
Backtrace
       |
       v
Identify function
       |
       v
Inspect variables
       |
       v
Find invalid memory access
```

---

# 37. StaticBox Debugging

For:

```text
StaticBox<50>
```

check:

```text
Capacity
Index handling
Entry insertion
Ordering
Boundary conditions
```

Important boundary tests:

```text
0 entries
1 entry
49 entries
50 entries
51 entries
```

The 51st insertion must follow the application's defined
overflow/error behavior.

---

# 38. DynamicBox Debugging

Check:

```text
vector growth
capacity
size
reallocation
iterator/reference validity
ordering
```

Test:

```text
0 entries
1 entry
100 entries
1000 entries
```

Observe:

```text
size()
capacity()
```

where useful.

---

# 39. Date Debugging

The Date class is critical because the storage system
depends on date ordering.

Test:

```text
2026-01-01
2026-02-01
2026-12-31
```

Invalid:

```text
2026-00-01
2026-13-01
2026-02-30
```

Comparison:

```text
2026-01-01 < 2026-01-02
```

Range:

```text
start <= date <= end
```

The exact inclusive/exclusive semantics should match the
implementation and tests.

---

# 40. Range Query Debugging

Example:

```text
Start = 2026-08-10
End   = 2026-08-18
```

Dataset:

```text
2026-08-01
2026-08-10
2026-08-12
2026-08-18
2026-08-20
```

Expected range result:

```text
2026-08-10
2026-08-12
2026-08-18
```

Debug:

```text
Input dates
    |
    v
Date comparison
    |
    v
Binary search
    |
    v
Start position
    |
    v
End position
    |
    v
Returned entries
```

---

# 41. Performance Debugging

The application is designed around sorted storage.

Lookup:

```text
O(log n)
```

Insertion may require shifting elements:

```text
O(n)
```

For large datasets, measure:

```text
Insertion time
Lookup time
Range-query time
Memory usage
```

Do not assume the README benchmark numbers are valid unless
they have been measured on the actual target hardware.

---

# 42. CPU and Memory Debugging

Check CPU:

```bash
top
```

or:

```bash
ps
```

Check memory:

```bash
free -h
```

Check process:

```bash
ps aux | grep boxes
```

Example:

```text
PID
CPU %
MEM %
COMMAND
```

---

# 43. File Descriptor Debugging

If the application uses files, sockets, or devices:

```bash
ls -l /proc/$(pidof boxes)/fd
```

This can identify open:

```text
Files
Devices
Pipes
Sockets
```

---

# 44. Environment Debugging

Check:

```bash
env
```

For the application process:

```bash
cat /proc/$(pidof boxes)/environ
```

This can help identify differences between:

```text
Manual execution
```

and:

```text
systemd execution
```

---

# 45. Manual vs systemd Debugging

This is a very useful diagnostic technique.

### Test 1

Run manually:

```bash
/opt/box-storage/bin/boxes demo
```

If it fails:

```text
Application / environment problem
```

### Test 2

Run through systemd:

```bash
systemctl restart box-storage.service
```

If manual execution works but systemd fails:

```text
Likely service/environment/permission/dependency problem
```

---

# 46. Complete Debugging Decision Tree

```text
                     Problem
                        |
                        v
                 Does board boot?
                   /          \
                 NO            YES
                 |              |
                 v              v
             Boot Debug     Does Linux start?
                            /            \
                           NO             YES
                           |               |
                           v               v
                       Kernel/DT       Does service start?
                                      /             \
                                     NO              YES
                                     |                |
                                     v                v
                                  systemd         Does app work?
                                                 /          \
                                                NO           YES
                                                |             |
                                                v             v
                                             C++/GDB       SUCCESS
                                             /logs
```

---

# 47. Debugging Command Summary

## Build

```bash
make clean
make
```

## Binary

```bash
file boxes
ldd boxes
```

## Application

```bash
./boxes demo
```

## Processes

```bash
ps aux
```

## Memory

```bash
free -h
```

## Kernel

```bash
dmesg
dmesg | tail -100
```

## Storage

```bash
lsblk
findmnt
df -h
```

## Systemd

```bash
systemctl status box-storage.service
systemctl restart box-storage.service
systemctl daemon-reload
```

## Logs

```bash
journalctl -u box-storage.service
journalctl -u box-storage.service -f
```

## Device Tree

```bash
ls /proc/device-tree
```

## Modules

```bash
lsmod
```

## GDB

```bash
gdb ./boxes
```

## Core Dumps

```bash
coredumpctl
```

---

# 48. Debugging Methodology

Always debug from the lowest failing layer upward.

```text
Hardware
   |
   v
Boot ROM
   |
   v
SPL
   |
   v
U-Boot
   |
   v
Kernel
   |
   v
Device Tree / Drivers
   |
   v
RootFS
   |
   v
systemd
   |
   v
Application
   |
   v
C++ Logic
```

Do not debug `BoxManager` if Linux itself has not booted.

Do not debug systemd if the executable is missing.

Do not debug the kernel if the application itself has a
reproducible C++ segmentation fault.

---

# 49. Debugging Evidence

For every failure, collect evidence before modifying code.

Useful evidence includes:

```text
1. Build output
2. file output
3. U-Boot console log
4. Kernel dmesg
5. systemctl status
6. journalctl output
7. Application output
8. GDB backtrace
9. Configuration files
10. Device Tree status
```

Example investigation:

```text
Problem:
box-storage.service failed

Evidence:
systemctl status
        |
        v
ExecStart failed

Check:
ls -l /opt/box-storage/bin/boxes
        |
        v
Binary missing

Root Cause:
Deployment/install step failed
```

This is much better than immediately changing C++ code.

---

# 50. Final Debugging Flow

The complete debugging process for this project is:

```text
                       FAILURE
                          |
                          v
                    Collect Logs
                          |
                          v
                   Identify Layer
                          |
         +----------------+----------------+
         |                |                |
         v                v                v
       Build            Boot           Runtime
         |                |                |
         v                v                v
      Compiler         U-Boot          systemd
      Linker           Kernel          Application
      Binary           Device Tree     GDB
                          |
                          v
                       RootFS
                          |
                          v
                       Drivers
                          |
                          v
                       Hardware
                          |
                          v
                      Root Cause
                          |
                          v
                         Fix
                          |
                          v
                       Rebuild
                          |
                          v
                       Retest
                          |
                          v
                      Deployment
                          |
                          v
                    Final Validation
```

---

# 51. Project Validation

A successful debug cycle ends with:

```text
Build
  |
  v
Unit Tests PASS
  |
  v
ARM Binary Valid
  |
  v
Image Generated
  |
  v
Board Boots
  |
  v
Linux Running
  |
  v
RootFS Mounted
  |
  v
systemd Running
  |
  v
box-storage.service ACTIVE
  |
  v
boxes Running
  |
  v
Add Operation PASS
  |
  v
Get Operation PASS
  |
  v
Range Query PASS
  |
  v
PROJECT VALIDATED
```

---

# 52. Interview Explanation

A concise explanation of the debugging approach is:

> "I debug the project layer by layer. I first verify compilation and the ARM binary, then check the bootloader and kernel through the serial console, verify the root filesystem and Device Tree, check systemd using `systemctl` and `journalctl`, and finally debug the C++ application using logs, GDB, AddressSanitizer and unit tests. This helps me identify the first failing layer instead of modifying code blindly."

````

So your `docs/` now has a very good **4-document engineering structure**:

```text
docs/
├── architecture.md   → WHAT is the system?
├── build_flow.md     → HOW is it built?
├── boot_flow.md      → HOW does the board boot it?
└── debugging.md      → HOW do we find and fix failures?
````

This is a strong structure for presenting the project in an embedded Linux/BSP interview.

