Yes. Add **`testing.md`** under `docs/`. This file should explain **how the BeagleBone Black project is tested from source code → host test → target test → output → pass/fail evaluation**.

Your final `docs/` structure becomes:

```text
docs/
├── architecture.md
├── boot_flow.md
├── build_flow.md
├── debugging.md
├── storage_flow.md
└── testing.md
```

# `docs/testing.md`

````markdown
# BeagleBone Black – TI AM3358
# Box Storage System – Testing Guide

## 1. Overview

This document describes the testing strategy for the Box Storage
System running on the BeagleBone Black based on the TI AM3358
Cortex-A8 processor.

The project is tested at multiple levels:

```text
Source Code
    |
    v
Unit Tests
    |
    v
Host Validation
    |
    v
Cross Compilation
    |
    v
BeagleBone Black
    |
    v
Target Execution
    |
    v
Functional Validation
    |
    v
Final Result
````

---

# 2. Testing Objectives

The testing process verifies:

1. Date parsing
2. Date validation
3. Date comparison
4. Entry creation
5. StaticBox insertion
6. DynamicBox insertion
7. StaticBox capacity handling
8. DynamicBox resizing
9. Sorted data ordering
10. Exact date lookup
11. Range queries
12. Invalid input handling
13. Application startup
14. Configuration loading
15. Systemd service startup
16. Application execution on BeagleBone Black

---

# 3. Test Directory

The project contains:

```text
app/
├── include/
│   ├── BoxManager.hpp
│   ├── Date.hpp
│   ├── DynamicBox.hpp
│   ├── Entry.hpp
│   └── StaticBox.hpp
│
├── src/
│   ├── BoxManager.cpp
│   ├── Date.cpp
│   ├── DynamicBox.cpp
│   ├── Entry.cpp
│   ├── StaticBox.cpp
│   └── main.cpp
│
└── tests/
    ├── test_date.cpp
    ├── test_dynamic_box.cpp
    ├── test_range_query.cpp
    └── test_static_box.cpp
```

Each test file focuses on a particular component.

---

# 4. Test Architecture

```text
                         Test System
                              |
              +---------------+---------------+
              |               |               |
              v               v               v
         Date Tests      StaticBox Tests  DynamicBox Tests
              |               |               |
              +---------------+---------------+
                              |
                              v
                       Range Query Tests
                              |
                              v
                         Test Result
```

---

# 5. Date Testing

File:

```text
app/tests/test_date.cpp
```

The Date test validates:

```text
String
  |
  v
Date::from_string()
  |
  v
Validation
  |
  v
Date object
```

---

# 6. Valid Date Test

Example:

```cpp
Date d = Date::from_string("2026-08-18");
```

Expected:

```text
Year  = 2026
Month = 8
Day   = 18
```

Test result:

```text
[PASS] Valid date parsing
```

---

# 7. Invalid Date Test

Examples:

```text
2026-13-01
2026-00-10
2026-02-30
```

Expected behavior:

```text
Invalid date
    |
    v
Exception / error
    |
    v
Test PASS
```

The exact error mechanism must match the implementation.

---

# 8. Date Comparison Testing

Example:

```text
Date A = 2026-08-10
Date B = 2026-08-18
```

Expected:

```text
A < B
```

Also test:

```text
A == A
A != B
B > A
```

This is important because Binary Search and Range Query depend
on correct date comparison.

---

# 9. StaticBox Testing

File:

```text
app/tests/test_static_box.cpp
```

The test verifies:

```text
StaticBox
   |
   +--> Add
   +--> Find
   +--> Ordering
   +--> Capacity
   +--> Error handling
```

---

# 10. StaticBox Add Test

Example:

```cpp
StaticBox<10> box;

box.add(
    Date::from_string("2026-08-18"),
    "Sensor data"
);
```

Expected:

```text
Add successful
```

Test result:

```text
[PASS] StaticBox add
```

---

# 11. StaticBox Multiple Entry Test

Add:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-18
```

Expected internal ordering:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-18
```

The test verifies that the sorted invariant is maintained.

---

# 12. StaticBox Unordered Input Test

Input:

```text
2026-08-18
2026-08-01
2026-08-10
2026-08-05
```

Expected storage:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-18
```

This is an important test because lookup relies on sorted data.

---

# 13. StaticBox Capacity Test

Suppose:

```cpp
StaticBox<3> box;
```

Add:

```text
Entry 1
Entry 2
Entry 3
```

Expected:

```text
size = 3
```

Then add:

```text
Entry 4
```

Expected behavior:

```text
Capacity exceeded
```

The test verifies that the implementation does not write beyond
the fixed capacity.

---

# 14. StaticBox Lookup Test

Data:

```text
2026-08-01 -> A
2026-08-10 -> B
2026-08-20 -> C
```

Search:

```text
2026-08-10
```

Expected:

```text
B
```

Test result:

```text
[PASS] StaticBox find
```

---

# 15. StaticBox Not Found Test

Search:

```text
2026-08-15
```

when only:

```text
2026-08-01
2026-08-10
2026-08-20
```

exist.

Expected:

```text
Not Found
```

The application must not return an incorrect neighboring entry.

---

# 16. DynamicBox Testing

File:

```text
app/tests/test_dynamic_box.cpp
```

The test verifies:

```text
DynamicBox
    |
    +--> Add
    +--> Dynamic growth
    +--> Ordering
    +--> Find
    +--> Large number of entries
```

---

# 17. DynamicBox Add Test

Example:

```cpp
DynamicBox box;

box.add(
    Date::from_string("2026-08-18"),
    "Sensor data"
);
```

Expected:

```text
[PASS] DynamicBox add
```

---

# 18. DynamicBox Resize Test

Start with a small number of entries.

Example:

```text
Entry 1
Entry 2
Entry 3
...
Entry 100
```

The vector should grow automatically.

Conceptually:

```text
Initial vector
     |
     v
Capacity reached
     |
     v
Reallocation
     |
     v
Larger storage
     |
     v
Continue insertion
```

Expected:

```text
100 entries successfully stored
```

---

# 19. DynamicBox Large Data Test

Insert:

```text
100
500
1000
10000
```

entries.

Verify:

```text
No corruption
No missing entries
Ordering preserved
Lookup works
Range query works
```

The actual test size depends on the project's intended limits.

---

# 20. DynamicBox Lookup Test

Example data:

```text
2026-08-01 -> Boot
2026-08-10 -> Sensor
2026-08-18 -> Camera
2026-08-20 -> Audio
```

Search:

```text
2026-08-18
```

Expected:

```text
Camera
```

---

# 21. Range Query Testing

File:

```text
app/tests/test_range_query.cpp
```

This is one of the most important tests.

Example data:

```text
2026-08-01 -> A
2026-08-05 -> B
2026-08-10 -> C
2026-08-15 -> D
2026-08-20 -> E
2026-08-25 -> F
```

Query:

```text
2026-08-10
to
2026-08-20
```

Expected:

```text
2026-08-10 -> C
2026-08-15 -> D
2026-08-20 -> E
```

The exact inclusion/exclusion behavior must match the project's
implementation and tests.

---

# 22. Range Query Boundary Test

Test:

```text
Start = 2026-08-10
End   = 2026-08-20
```

Verify specifically:

```text
Start date behavior
End date behavior
Dates before start
Dates after end
```

This catches common comparison mistakes.

---

# 23. Empty Range Test

Example:

```text
Start = 2026-09-01
End   = 2026-09-10
```

when the database contains only August data.

Expected:

```text
No entries found
```

---

# 24. Invalid Range Test

Example:

```text
Start = 2026-08-20
End   = 2026-08-10
```

Expected behavior:

```text
Invalid range
```

The exact behavior should follow the application's API contract.

---

# 25. Duplicate Entry Test

Test inserting the same date twice:

```text
2026-08-18 -> A
2026-08-18 -> B
```

The test must verify the project's intended duplicate-date behavior.

Possible results:

```text
Reject
```

or:

```text
Allow multiple entries
```

or:

```text
Replace existing entry
```

Do not assume the behavior without checking the implementation.

---

# 26. Complete Unit Test Flow

```text
                 Source Code
                     |
          +----------+----------+
          |          |          |
          v          v          v
       Date      StaticBox   DynamicBox
       Tests       Tests       Tests
          |          |          |
          +----------+----------+
                     |
                     v
               Range Tests
                     |
                     v
                Test Runner
                     |
             +-------+-------+
             |               |
             v               v
           PASS             FAIL
             |               |
             v               v
        Continue          Debug Code
```

---

# 27. Building Tests

From the project root:

```bash
make clean
make
make test
```

If the Makefile provides a test target, it should build the
test executables and execute them.

Example expected output:

```text
[PASS] Date parsing
[PASS] Date comparison
[PASS] StaticBox add/find
[PASS] StaticBox capacity
[PASS] DynamicBox add
[PASS] DynamicBox resize
[PASS] Range query
```

The exact output depends on the project's test framework and
Makefile.

---

# 28. Manual Test

After unit tests pass, run the application manually.

Example:

```bash
./bin/boxes demo
```

Expected:

```text
Box Storage System
------------------

StaticBox:
...

DynamicBox:
...

Range Query:
...

All operations completed successfully.
```

The actual output depends on `main.cpp`.

---

# 29. Functional Test

Test the command-line interface.

### Add

```bash
./bin/boxes add static 2026-08-18 "Sensor data"
```

Expected:

```text
Entry added successfully
```

### Get

```bash
./bin/boxes get static 2026-08-18
```

Expected:

```text
2026-08-18
Sensor data
```

### Range

```bash
./bin/boxes range static 2026-08-01 2026-08-31
```

Expected:

```text
All matching entries
```

The exact syntax must match the implementation in `main.cpp`.

---

# 30. Host Testing

Before deploying to the BeagleBone Black, tests should ideally
be executed on the development machine.

```text
Developer PC
    |
    v
Compile
    |
    v
Unit Tests
    |
    v
PASS
    |
    v
Cross Compile
    |
    v
BeagleBone Black
```

This makes debugging much faster.

---

# 31. Cross Compilation

The BeagleBone Black uses:

```text
TI AM3358
ARM Cortex-A8
```

Therefore, the target executable may need to be built using an
ARM cross compiler.

Conceptually:

```bash
arm-linux-gnueabihf-g++ ...
```

The exact compiler depends on the selected BeagleBone/Yocto
toolchain.

The important distinction is:

```text
Host:
x86_64

Target:
ARM
```

---

# 32. Cross-Compile Test Flow

```text
C++ Source
    |
    v
ARM Cross Compiler
    |
    v
ARM Executable
    |
    v
Copy to BeagleBone
    |
    v
Execute on Target
```

---

# 33. Copy Application to BeagleBone

Example:

```bash
scp bin/boxes debian@<BOARD_IP>:/tmp/
```

Then:

```bash
ssh debian@<BOARD_IP>
```

Run:

```bash
/tmp/boxes demo
```

The exact username depends on the target Linux image.

---

# 34. Target Testing

On the BeagleBone:

```bash
uname -m
```

Verify the architecture.

Example:

```text
armv7l
```

Then:

```bash
/tmp/boxes demo
```

Verify that the ARM executable executes correctly.

---

# 35. Target Functional Test

Run:

```bash
/tmp/boxes add dynamic 2026-08-18 "Sensor data"
```

Then:

```bash
/tmp/boxes get dynamic 2026-08-18
```

Expected:

```text
2026-08-18
Sensor data
```

Then:

```bash
/tmp/boxes range dynamic 2026-08-01 2026-08-31
```

Verify the expected entries are returned.

---

# 36. Configuration Testing

Configuration files:

```text
config/
├── boxes.conf
└── logging.conf
```

Verify that they are installed correctly:

```bash
ls -l /etc/box-storage/
```

or the project's configured installation path.

Then start the application and verify that configuration
parameters are loaded correctly.

---

# 37. Systemd Testing

The service file is:

```text
deployment/systemd/box-storage.service
```

After installation:

```bash
systemctl status box-storage.service
```

Expected:

```text
Active: active (running)
```

If it is not running:

```bash
journalctl -u box-storage.service
```

Use the logs to identify the failure.

---

# 38. Boot-Time Testing

The final target test should verify:

```text
Power ON
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
boxes application
   |
   v
Application Ready
```

This verifies that the project works after a complete board
reboot rather than only when manually started.

---

# 39. Reboot Test

Execute:

```bash
sudo reboot
```

After reconnecting:

```bash
systemctl status box-storage.service
```

Verify:

```text
Service started successfully
```

Then execute the application's functional tests again.

---

# 40. Power-Cycle Test

A stronger board-level test is:

```text
Power OFF
    |
    v
Power ON
    |
    v
Boot
    |
    v
Application Start
    |
    v
Functional Test
```

This catches boot-time and initialization problems that a simple
application restart may not detect.

---

# 41. Memory Testing

For DynamicBox, monitor memory usage.

Example:

```bash
free -h
```

Process-level information:

```bash
top
```

or:

```bash
ps aux | grep boxes
```

If available:

```bash
valgrind ./bin/boxes demo
```

Valgrind is generally more useful on a supported development
environment than directly on the constrained target.

---

# 42. Memory Leak Testing

If Valgrind is available:

```bash
valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    ./bin/boxes demo
```

Look for:

```text
definitely lost: 0 bytes
indirectly lost: 0 bytes
```

The exact expected result depends on all linked libraries and
the test program's shutdown behavior.

---

# 43. Stress Testing

Stress testing evaluates the system with a large number of
entries.

Example:

```text
1,000 entries
5,000 entries
10,000 entries
```

Verify:

```text
Insertion succeeds
Ordering remains correct
Lookup remains correct
Range query remains correct
Memory remains stable
No crashes
```

---

# 44. Performance Testing

Measure:

```text
Add
Get
Range Query
```

Example concept:

```text
Start timer
    |
    v
Perform operation N times
    |
    v
Stop timer
    |
    v
Calculate average
```

For example:

```text
Average lookup time
=
Total lookup time / Number of lookups
```

Do not treat benchmark numbers in the README as guaranteed
hardware-independent results; they must be measured on the
specific build and target.

---

# 45. Test Matrix

| Test               |     Host | BeagleBone |
| ------------------ | -------: | ---------: |
| Date parsing       |      Yes |        Yes |
| Date comparison    |      Yes |        Yes |
| StaticBox add      |      Yes |        Yes |
| StaticBox capacity |      Yes |        Yes |
| DynamicBox resize  |      Yes |        Yes |
| Exact lookup       |      Yes |        Yes |
| Range query        |      Yes |        Yes |
| Invalid input      |      Yes |        Yes |
| CLI                |      Yes |        Yes |
| Configuration      | Optional |        Yes |
| systemd            |       No |        Yes |
| Boot test          |       No |        Yes |
| Power-cycle test   |       No |        Yes |
| Memory test        |      Yes |        Yes |
| Performance        |      Yes |        Yes |

---

# 46. Test Levels

The complete testing hierarchy is:

```text
Level 1
Unit Testing
    |
    v
Level 2
Component Testing
    |
    v
Level 3
Application Testing
    |
    v
Level 4
Target Testing
    |
    v
Level 5
System Testing
    |
    v
Level 6
Boot / Power-Cycle Testing
```

---

# 47. Failure Debugging Flow

If a test fails:

```text
Test Failure
     |
     v
Identify Test
     |
     v
Identify Component
     |
     +------ Date
     |
     +------ StaticBox
     |
     +------ DynamicBox
     |
     +------ Range Query
     |
     +------ CLI
     |
     +------ systemd
     |
     v
Check Logs
     |
     v
Debug Source
     |
     v
Rebuild
     |
     v
Run Test Again
```

---

# 48. Example Failure

Suppose:

```bash
./bin/boxes get dynamic 2026-08-18
```

returns:

```text
Not Found
```

but the entry exists.

Debug:

```text
CLI input
   |
   v
Date parsing
   |
   v
Date comparison
   |
   v
DynamicBox contents
   |
   v
Sorted ordering
   |
   v
Binary search
```

Check each layer.

---

# 49. Test Evidence

For project evaluation, collect:

```text
1. Build output
2. Unit test output
3. Application output
4. Target execution output
5. systemd status
6. Kernel architecture
7. Boot logs
8. Memory test results
9. Functional test results
10. Performance measurements
```

Example:

```bash
uname -a
```

```bash
systemctl status box-storage.service
```

```bash
journalctl -u box-storage.service
```

```bash
./bin/boxes demo
```

---

# 50. Final Acceptance Criteria

The project is considered successfully tested when:

```text
[PASS] Source compilation
[PASS] Unit tests
[PASS] Date validation
[PASS] StaticBox operations
[PASS] DynamicBox operations
[PASS] Sorted storage
[PASS] Binary search
[PASS] Range query
[PASS] Invalid input handling
[PASS] ARM cross compilation
[PASS] BeagleBone execution
[PASS] Configuration loading
[PASS] systemd startup
[PASS] Reboot test
[PASS] Functional test
```

---

# 51. Complete Testing Flow

```text
                     SOURCE CODE
                          |
                          v
                    Build on Host
                          |
                          v
                    Unit Testing
                          |
              +-----------+-----------+
              |           |           |
              v           v           v
             Date      StaticBox   DynamicBox
              |           |           |
              +-----------+-----------+
                          |
                          v
                     Range Query
                          |
                          v
                    All Tests PASS
                          |
                          v
                    Cross Compile
                          |
                          v
                    ARM Binary
                          |
                          v
                  Deploy to Target
                          |
                          v
                BeagleBone Black
                          |
                          v
                       Linux
                          |
                          v
                       systemd
                          |
                          v
                  boxes application
                          |
                          v
                  Functional Tests
                          |
                          v
                    Reboot Test
                          |
                          v
                  Power-Cycle Test
                          |
                          v
                  FINAL ACCEPTANCE
```

---

# 52. Test Result Example

A successful evaluation can be summarized as:

```text
========================================
       BOX STORAGE SYSTEM TEST
========================================

Date Tests              : PASS
StaticBox Tests         : PASS
DynamicBox Tests        : PASS
Range Query Tests       : PASS
CLI Tests               : PASS
ARM Build               : PASS
Target Execution        : PASS
Configuration           : PASS
systemd Service         : PASS
Reboot Test             : PASS
Power Cycle Test        : PASS

========================================
             RESULT: PASS
========================================
```

---

# 53. Relationship With Other Documentation

The documentation flow is:

```text
architecture.md
       |
       v
How the software is structured
       |
       v
build_flow.md
       |
       v
How source becomes executable
       |
       v
boot_flow.md
       |
       v
How BeagleBone boots the software
       |
       v
storage_flow.md
       |
       v
How application data is stored
       |
       v
testing.md
       |
       v
How everything is validated
       |
       v
debugging.md
       |
       v
How failures are diagnosed
```

This makes `testing.md` the **validation document** for the entire BeagleBone Black project, while `storage_flow.md` explains the runtime data path and `boot_flow.md` explains how the target reaches the application.

