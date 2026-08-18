# Box Storage System Architecture

## 1. Project Overview

The Box Storage System is a C++17 date-indexed key-value storage application
designed for the BeagleBone Black platform using the TI AM3358 processor.

The application supports two storage implementations:

- StaticBox
- DynamicBox

The system stores timestamped string data and provides date-based lookup
and range-query functionality.

---

# 2. Target Hardware

## BeagleBone Black

| Component | Specification |
|---|---|
| SoC | TI AM3358 |
| CPU | ARM Cortex-A8 |
| Architecture | ARMv7-A |
| Storage | eMMC / microSD |
| OS | Embedded Linux |
| Application | C++17 |
| Build | Cross compilation |

---

# 3. High-Level Architecture

```text
                        BeagleBone Black
                         TI AM3358
                              |
                              |
                         Embedded Linux
                              |
                              v
                           systemd
                              |
                              v
                    box-storage.service
                              |
                              v
                     Box Storage App
                              |
                         main.cpp
                              |
                              v
                       BoxManager
                              |
                 +------------+------------+
                 |                         |
                 v                         v
             StaticBox                DynamicBox
                 |                         |
                 |                         |
            std::array               std::vector
                 |                         |
                 +------------+------------+
                              |
                              v
                            Entry
                              |
                              v
                 
