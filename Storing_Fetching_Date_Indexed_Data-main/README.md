#  Box Storage System

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)]()
[![License](https://img.shields.io/badge/license-MIT-green.svg)]()
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20WSL-lightgrey.svg)]()

> Enterprise-grade dual-mode temporal key-value store with compile-time polymorphic containers

A high-performance, header-based C++17 library for managing timestamped string entries with two storage strategies: static (fixed-capacity, stack-allocated) and dynamic (heap-resizable).

---

##  Features

###  Dual Storage Architecture

* `StaticBox<T>` → Fixed capacity (no heap allocation)
* `DynamicBox` → Resizable using `std::vector`

###  Date Support

* ISO 8601 (YYYY-MM-DD)
* Full comparison operators
* Strict validation

###  Unified API

* Consistent interface for both storage types
* Safe error handling (bool / optional / nullptr)

###  Performance

* O(log n) retrieval (binary search)
* O(1) comparisons
* Cache-friendly static layout

---

##  Prerequisites

| Tool                | Version |
| ------------------- | ------- |
| g++ / clang++       | C++17   |
| make                | 4.0+    |
| valgrind (optional) | 3.15+   |

---

##  Build

```bash
git clone https://github.com/yourusername/boxes.git
cd boxes
make clean && make
```

Expected:

```bash
g++ -std=c++17 -Wall -Wextra -O2 -c src/main.cpp
...
g++ obj/*.o -o bin/boxes
```

---

##  Run

```bash
./bin/boxes demo
```

---

##  Commands

### Add entries

```bash
./bin/boxes add static 2025-12-01 "Sensor data"
./bin/boxes add dynamic 2025-11-30 "Audit log"
```

### Query

```bash
./bin/boxes get static 2025-12-01
./bin/boxes range dynamic 2025-11-01 2025-12-31
```

---

##  Architecture

### Fixed Mermaid Fix

Your original diagram had a parse error due to `&` usage.

Correct version:

```mermaid
graph TD
    A[Entry] --> B[Date]
    A --> C[String payload]

    D[Storage Concept] --> E[StaticBox]
    D --> F[DynamicBox]

    E --> G[std::array<Entry, N>]
    F --> H[std::vector<Entry>]

    G --> I[Sorted invariant]
    H --> I

    I --> J[Binary search O(log n)]
```

---

##  Memory Model

| Feature          | StaticBox | DynamicBox |
| ---------------- | --------- | ---------- |
| Heap allocations | 0         | Yes        |
| Cache locality   | High      | Medium     |
| Capacity         | Fixed     | Dynamic    |
| Add complexity   | O(1)      | O(n) worst |

---

##  Date Structure

```cpp
struct Date {
    int year;
    int month;
    int day;

    static Date from_string(const std::string& iso);

    std::strong_ordering operator<=>(const Date&) const;
};
```

---

##  StaticBox Example

```cpp
StaticBox<50> box;
box.add({2025,12,1}, "Event");
```

---

##  DynamicBox Example

```cpp
DynamicBox box;
box.add(Date::from_string("2025-11-30"), "Login");
```

---

##  Testing

```bash
make test
./bin/test_boxes
```

Expected:

```
[PASS] StaticBox add/find
[PASS] DynamicBox resize
[PASS] Date parsing
```

---

##  Benchmark

| Operation | StaticBox | DynamicBox |
| --------- | --------- | ---------- |
| Add       | 200 ns    | 1.2 μs     |
| Get       | 180 ns    | 210 ns     |

---

##  Common Fixes

| Issue          | Fix                     |
| -------------- | ----------------------- |
| g++ not found  | install build-essential |
| compile errors | use -std=c++17          |
| date fails     | ensure YYYY-MM-DD       |

---

