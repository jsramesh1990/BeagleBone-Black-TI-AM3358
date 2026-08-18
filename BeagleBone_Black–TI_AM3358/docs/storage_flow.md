Yes. Add **`storage_flow.md`**. For this project, this should explain the **actual data-storage path**: application → `BoxManager` → `StaticBox`/`DynamicBox` → `Entry` → `Date` → memory, plus how data is retrieved and how this differs from persistent SD/eMMC storage.

Your `docs/` becomes:

```text
docs/
├── architecture.md
├── boot_flow.md
├── build_flow.md
├── debugging.md
└── storage_flow.md
```

# `docs/storage_flow.md`

````markdown
# BeagleBone Black TI AM3358 - Storage Flow

## 1. Overview

This document describes how timestamped data moves through the
Box Storage System running on the BeagleBone Black.

The application provides two storage mechanisms:

```text
StaticBox
    |
    +--> Fixed capacity
    +--> std::array
    +--> No dynamic heap allocation for the container

DynamicBox
    |
    +--> Resizable capacity
    +--> std::vector
    +--> Heap-backed storage
````

The complete application data flow is:

```text
User / Application Input
          |
          v
      main.cpp
          |
          v
     BoxManager
          |
      +---+---+
      |       |
      v       v
 StaticBox DynamicBox
      |       |
      +---+---+
          |
          v
        Entry
          |
       +--+--+
       |     |
       v     v
      Date Payload
       |
       v
   Sorted Storage
       |
       v
 Binary Search / Range Query
       |
       v
    Result
```

---

# 2. Important Storage Concept

There are two different meanings of "storage" in this project.

## Application Data Storage

The C++ containers store entries in RAM:

```text
Application
     |
     v
StaticBox / DynamicBox
     |
     v
RAM
```

This is the primary storage mechanism implemented by the
Box Storage System.

## Persistent Board Storage

The BeagleBone itself has persistent storage:

```text
SD Card
   or
eMMC
```

This stores:

```text
Bootloader
Linux Kernel
Device Tree
RootFS
Application
Configuration
```

These are different concepts.

```text
                 BeagleBone
                     |
          +----------+----------+
          |                     |
          v                     v
      Persistent              Runtime
       Storage                Memory
          |                     |
       SD/eMMC              StaticBox
                              |
                           DynamicBox
                              |
                              v
                             RAM
```

The current Box Storage application should not be described as
an SD/eMMC database unless persistent data handling has actually
been implemented.

---

# 3. Data Model

The basic stored object is an `Entry`.

Conceptually:

```cpp
struct Entry {
    Date date;
    std::string payload;
};
```

Each entry contains:

```text
+-----------------------------+
| Entry                       |
+-----------------------------+
| Date                        |
|   year                      |
|   month                     |
|   day                       |
+-----------------------------+
| Payload                     |
|   "Sensor data"             |
+-----------------------------+
```

Example:

```text
Date:
2026-08-18

Payload:
"Temperature = 32 C"
```

---

# 4. Date Representation

The `Date` class represents the timestamp/index.

Files:

```text
app/include/Date.hpp
app/src/Date.cpp
```

Conceptually:

```cpp
struct Date {
    int year;
    int month;
    int day;
};
```

Example:

```text
2026-08-18
```

Internally:

```text
year  = 2026
month = 8
day   = 18
```

---

# 5. Date Parsing Flow

When a string is received:

```text
"2026-08-18"
```

the flow is:

```text
String
  |
  v
Date::from_string()
  |
  v
Validate format
  |
  v
Validate year
  |
  v
Validate month
  |
  v
Validate day
  |
  v
Date object
```

Example:

```cpp
Date date = Date::from_string("2026-08-18");
```

Result:

```text
Date
├── year  = 2026
├── month = 8
└── day   = 18
```

---

# 6. Date Validation

The date parser should reject invalid dates.

Examples:

```text
2026-08-18    VALID
2026-01-01    VALID
2026-12-31    VALID
```

Invalid examples:

```text
2026-13-01
2026-00-10
2026-02-30
```

The exact validation rules are determined by the implementation
in `Date.cpp`.

---

# 7. Entry Creation

The application creates an entry from:

```text
Date
+
Payload
```

Example:

```cpp
Entry entry{
    Date::from_string("2026-08-18"),
    "Temperature = 32 C"
};
```

Conceptually:

```text
Input
 |
 +---- Date
 |
 +---- Payload
 |
 v
Entry
```

---

# 8. BoxManager

`BoxManager` provides the higher-level management layer.

Files:

```text
app/include/BoxManager.hpp
app/src/BoxManager.cpp
```

Conceptually:

```text
                 BoxManager
                     |
             +-------+-------+
             |               |
             v               v
         StaticBox       DynamicBox
```

Its responsibility is to coordinate storage operations rather
than making `main.cpp` directly manage every container operation.

---

# 9. StaticBox Storage

Static storage uses a fixed-capacity container.

Conceptually:

```cpp
StaticBox<50> box;
```

Internally:

```text
StaticBox
+--------------------------------+
| Entry 0                        |
+--------------------------------+
| Entry 1                        |
+--------------------------------+
| Entry 2                        |
+--------------------------------+
| ...                            |
+--------------------------------+
| Entry 49                       |
+--------------------------------+
```

Capacity:

```text
50 entries
```

The exact capacity is determined by the template parameter.

---

# 10. StaticBox Memory Model

The conceptual memory arrangement is:

```text
Stack / Object Storage
        |
        v
+-----------------------+
| StaticBox             |
+-----------------------+
| Entry                 |
| Entry                 |
| Entry                 |
| Entry                 |
| ...                   |
+-----------------------+
```

The container itself does not need to dynamically grow.

However, individual members such as `std::string` may have
their own memory behavior. Therefore, "no heap allocation"
should be interpreted as a property of the container's storage
strategy, not necessarily a guarantee that absolutely no heap
allocation occurs anywhere inside every `Entry`.

---

# 11. DynamicBox Storage

Dynamic storage uses:

```cpp
std::vector<Entry>
```

Conceptually:

```text
DynamicBox
     |
     v
std::vector
     |
     v
Heap
     |
     +---- Entry
     +---- Entry
     +---- Entry
     +---- Entry
```

Unlike `StaticBox`, the number of entries can grow dynamically.

---

# 12. DynamicBox Growth

Suppose the vector has:

```text
size     = 4
capacity = 4
```

Adding another entry may require reallocation:

```text
Old memory
+---+---+---+---+
| E | E | E | E |
+---+---+---+---+

        |
        | resize/reallocation
        v

New memory
+---+---+---+---+---+---+---+---+
| E | E | E | E |   |   |   |   |
+---+---+---+---+---+---+---+---+
```

The exact capacity-growth policy is controlled by the C++
standard library implementation.

---

# 13. Adding Data

Example command:

```text
add dynamic 2026-08-18 "Sensor data"
```

The conceptual flow is:

```text
Command Line
     |
     v
main.cpp
     |
     v
Parse arguments
     |
     v
Date::from_string()
     |
     v
Create Entry
     |
     v
BoxManager
     |
     v
DynamicBox::add()
     |
     v
std::vector<Entry>
     |
     v
Sorted Storage
```

For static storage:

```text
Command
  |
  v
main.cpp
  |
  v
BoxManager
  |
  v
StaticBox::add()
  |
  v
Fixed Storage
```

---

# 14. Storage Invariant

The important design rule is:

```text
Entries are maintained in chronological order.
```

Example:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-18
2026-08-25
```

Not:

```text
2026-08-18
2026-08-01
2026-08-25
2026-08-05
```

Maintaining sorted data enables efficient searching.

---

# 15. Insertion Flow

Suppose the current storage is:

```text
2026-08-01
2026-08-10
2026-08-20
```

Insert:

```text
2026-08-15
```

The conceptual operation is:

```text
Existing:

2026-08-01
2026-08-10
2026-08-20

             ^
             |
          Insert

After:

2026-08-01
2026-08-10
2026-08-15
2026-08-20
```

The exact insertion algorithm should be taken from
`StaticBox.cpp` and `DynamicBox.cpp`.

---

# 16. Why Sorted Storage?

Sorted storage enables binary search.

Without sorting:

```text
Entry 0
Entry 1
Entry 2
...
Entry N
```

A lookup may require:

```text
O(n)
```

With sorted storage:

```text
Binary Search
     |
     v
O(log n)
```

Conceptually:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-15
2026-08-20
2026-08-25
2026-08-30
```

Search:

```text
2026-08-20
```

Binary search repeatedly reduces the search space.

---

# 17. Get Operation

Example:

```text
get dynamic 2026-08-18
```

Flow:

```text
User
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
Binary Search
 |
 v
Date Comparison
 |
 +---- Found
 |       |
 |       v
 |     Entry
 |
 +---- Not Found
         |
         v
       Result
```

---

# 18. Date Comparison

The search depends on Date comparison.

Conceptually:

```text
Date A
   |
   v
Compare
   ^
   |
Date B
```

Example:

```text
2026-08-10 < 2026-08-18
```

Therefore:

```text
2026-08-10
comes before
2026-08-18
```

The implementation may use C++ comparison operators,
including `<=>` where supported by the project's C++ version.

---

# 19. Binary Search Flow

Suppose:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-15
2026-08-20
2026-08-25
2026-08-30
```

Search:

```text
2026-08-20
```

Conceptually:

```text
              Middle
                |
                v
2026-08-15

Target is greater
        |
        v

Search right half

2026-08-20
```

Continue until:

```text
Found
```

or:

```text
Not Found
```

---

# 20. Range Query

Range queries are one of the important features.

Example:

```text
range dynamic 2026-08-10 2026-08-20
```

Flow:

```text
Start Date
     |
     v
End Date
     |
     v
BoxManager
     |
     v
DynamicBox
     |
     v
Find start position
     |
     v
Find end position
     |
     v
Return entries
```

---

# 21. Range Example

Stored data:

```text
2026-08-01
2026-08-05
2026-08-10
2026-08-15
2026-08-20
2026-08-25
```

Query:

```text
2026-08-10 -> 2026-08-20
```

Result:

```text
2026-08-10
2026-08-15
2026-08-20
```

The exact boundary behavior must match the implementation
and tests.

---

# 22. Range Query Algorithm

Conceptually:

```text
Sorted Entries
      |
      v
Find first entry >= start
      |
      v
Find first entry > end
      |
      v
Return [start, end)
```

or whatever boundary convention the implementation uses.

The actual project code and tests are the authority for the
precise semantics.

---

# 23. StaticBox Data Flow

Complete StaticBox flow:

```text
Input
  |
  v
main.cpp
  |
  v
BoxManager
  |
  v
StaticBox
  |
  v
Fixed Entry Storage
  |
  v
Date + Payload
  |
  v
Sorted Order
  |
  v
Binary Search
  |
  v
Result
```

---

# 24. DynamicBox Data Flow

Complete DynamicBox flow:

```text
Input
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
std::vector<Entry>
  |
  v
Heap Memory
  |
  v
Sorted Order
  |
  v
Binary Search
  |
  v
Result
```

---

# 25. Static vs Dynamic

| Feature         | StaticBox               | DynamicBox                 |
| --------------- | ----------------------- | -------------------------- |
| Container       | Fixed-size storage      | `std::vector`              |
| Capacity        | Fixed                   | Dynamic                    |
| Growth          | No automatic growth     | Automatic vector growth    |
| Memory strategy | Fixed container storage | Heap-backed vector storage |
| Lookup          | O(log n), if sorted     | O(log n), if sorted        |
| Insert          | Potentially O(n)        | Potentially O(n)           |
| Use case        | Known bounded data      | Variable data volume       |

---

# 26. RAM Layout

At runtime:

```text
                 Process Memory
                       |
       +---------------+---------------+
       |               |               |
       v               v               v
     Code            Stack            Heap
       |               |               |
       |               |               |
       |           Local objects    DynamicBox
       |                               |
       |                               v
       |                         vector storage
       |                               |
       |                               v
       |                             Entries
       |
       v
    Functions
```

The exact placement of an object depends on how it is created.

For example:

```cpp
StaticBox<50> box;
```

creates an automatic local object when declared as a local
variable, while:

```cpp
DynamicBox box;
```

contains a vector whose element storage is dynamically allocated
as the vector grows.

---

# 27. What Happens During Add?

Example:

```text
Date:
2026-08-18

Payload:
"Temperature=32"
```

Flow:

```text
1. Receive date string
        |
        v
2. Parse date
        |
        v
3. Validate date
        |
        v
4. Create Entry
        |
        v
5. Select StaticBox/DynamicBox
        |
        v
6. Find insertion position
        |
        v
7. Insert Entry
        |
        v
8. Maintain sorted order
```

---

# 28. What Happens During Get?

Example:

```text
get dynamic 2026-08-18
```

Flow:

```text
1. Parse date
        |
        v
2. Validate date
        |
        v
3. Select DynamicBox
        |
        v
4. Binary search
        |
        v
5. Compare Date
        |
        +---- Match
        |      |
        |      v
        |    Return Entry
        |
        +---- No match
               |
               v
            Not Found
```

---

# 29. What Happens During Range?

Example:

```text
range dynamic 2026-08-01 2026-08-18
```

Flow:

```text
Start Date
    |
    v
Parse
    |
    v
Validate
    |
    v
End Date
    |
    v
Parse
    |
    v
Validate
    |
    v
Find range
    |
    v
Return matching Entries
```

---

# 30. Error Handling

Possible storage errors include:

```text
Invalid Date
Duplicate Date
StaticBox Full
Entry Not Found
Invalid Range
Invalid Command
```

The project API may represent errors using:

```text
bool
std::optional
nullptr
```

depending on the function.

The actual behavior should be verified against the implementation.

---

# 31. StaticBox Capacity Error

Example:

```text
StaticBox capacity = 50
```

If 50 entries are already stored:

```text
size = 50
capacity = 50
```

Adding another entry must follow the implementation's defined
overflow behavior.

Conceptually:

```text
51st entry
    |
    v
Capacity check
    |
    v
FULL
    |
    v
Return failure
```

---

# 32. DynamicBox Capacity

DynamicBox does not have the same fixed logical capacity as
StaticBox.

Conceptually:

```text
Entry 1
Entry 2
Entry 3
...
Entry N
```

The vector grows as needed, subject to available memory and
implementation/system limits.

---

# 33. Duplicate Dates

The behavior for duplicate dates depends on the project
implementation.

Possible designs include:

```text
2026-08-18 -> Sensor A
2026-08-18 -> Sensor B
```

or:

```text
Duplicate date -> reject
```

or:

```text
Duplicate date -> replace
```

Do not assume which behavior is implemented.

The source and unit tests should define the actual project
behavior.

---

# 34. Configuration Storage

Configuration files are located under:

```text
config/
├── boxes.conf
└── logging.conf
```

These are persistent files on the target filesystem after
deployment.

Conceptually:

```text
RootFS
 |
 +--> /opt/box-storage/bin/boxes
 |
 +--> /opt/box-storage/config/boxes.conf
 |
 +--> /opt/box-storage/config/logging.conf
```

Configuration is different from the in-memory Entry storage.

---

# 35. SD/eMMC Storage

The deployment image stores the software persistently.

Conceptually:

```text
                 SD / eMMC
                     |
        +------------+------------+
        |            |            |
        v            v            v
    Bootloader     Kernel       RootFS
                                  |
                    +-------------+-------------+
                    |             |             |
                    v             v             v
                  boxes       config        systemd
```

After Linux boots:

```text
SD/eMMC
   |
   v
RootFS
   |
   v
Application loaded
   |
   v
Runtime objects created in RAM
```

---

# 36. Important Persistence Limitation

If the application only stores entries inside:

```text
StaticBox
```

or:

```text
DynamicBox
```

then the data is normally lost when the process exits or
the board reboots.

For example:

```text
Power ON
   |
   v
Application starts
   |
   v
Add Entry
   |
   v
RAM
   |
   v
Power OFF
   |
   v
RAM lost
```

Persistent data would require an additional storage layer.

---

# 37. Persistent Storage Extension

If persistence is required, the architecture could be extended:

```text
                    BoxManager
                        |
             +----------+----------+
             |                     |
             v                     v
        In-Memory Box          Persistent Store
             |                     |
             v                     v
            RAM                  SD/eMMC
```

Possible persistent formats include:

```text
Binary file
CSV
JSON
SQLite
Custom database format
```

The current project should only claim persistence if such a
mechanism is actually implemented.

---

# 38. Save Flow With Persistence

If persistence is later added:

```text
Entry
  |
  v
BoxManager
  |
  +------------+
  |            |
  v            v
RAM           File
  |            |
  v            v
Static/      SD/eMMC
Dynamic
```

---

# 39. Load Flow With Persistence

At application startup:

```text
Application Start
       |
       v
Read persistent storage
       |
       v
Parse Entries
       |
       v
Validate Dates
       |
       v
Populate Box
       |
       v
Sort / Verify invariant
       |
       v
Application Ready
```

This would provide persistence across reboot.

---

# 40. End-to-End Storage Flow

The complete current architecture is:

```text
                  USER INPUT
                      |
                      v
                  main.cpp
                      |
                      v
                 BoxManager
                      |
              +-------+-------+
              |               |
              v               v
          StaticBox       DynamicBox
              |               |
              |               v
              |          std::vector
              |               |
              +-------+-------+
                      |
                      v
                    Entry
                      |
                +-----+-----+
                |           |
                v           v
              Date       Payload
                |
                v
          Sorted Entries
                |
                v
          Binary Search
                |
         +------+------+
         |             |
         v             v
       Get          Range Query
         |             |
         +------+------+
                |
                v
             Result
```

---

# 41. Storage vs Boot Storage

Do not confuse these two flows.

## Boot Storage

```text
SD/eMMC
   |
   v
Bootloader
   |
   v
Kernel
   |
   v
RootFS
```

## Application Data Storage

```text
Application
   |
   v
BoxManager
   |
   v
StaticBox/DynamicBox
   |
   v
RAM
```

They are independent layers.

---

# 42. Complete Board-Level Flow

```text
                SD / eMMC
                    |
                    v
              Bootloader
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
                boxes
                    |
                    v
              BoxManager
                    |
          +---------+---------+
          |                   |
          v                   v
      StaticBox           DynamicBox
          |                   |
          +---------+---------+
                    |
                    v
                  Entry
                    |
             +------+------+
             |             |
             v             v
            Date        Payload
             |
             v
         Sorted RAM
             |
      +------+------+
      |             |
      v             v
     Get        Range Query
```

---

# 43. Storage Test Flow

The storage implementation is evaluated through:

```text
app/tests/
├── test_date.cpp
├── test_dynamic_box.cpp
├── test_range_query.cpp
└── test_static_box.cpp
```

Testing flow:

```text
Source
  |
  v
Build
  |
  v
Date Tests
  |
  v
StaticBox Tests
  |
  v
DynamicBox Tests
  |
  v
Range Query Tests
  |
  v
All PASS
```

---

# 44. Storage Validation

The storage system should be validated for:

```text
1. Date parsing
2. Date validation
3. Date comparison
4. Entry creation
5. Entry insertion
6. Sorted ordering
7. Static capacity
8. Dynamic growth
9. Exact lookup
10. Range lookup
11. Not-found behavior
12. Invalid-input handling
```

---

# 45. Debugging Storage

If a lookup returns the wrong entry:

```text
Input
  |
  v
Date parsing
  |
  v
Date comparison
  |
  v
Insertion ordering
  |
  v
Sorted invariant
  |
  v
Binary search
  |
  v
Result
```

Check each stage.

If range query is incorrect:

```text
Start Date
    |
    v
End Date
    |
    v
Boundary logic
    |
    v
Binary search
    |
    v
Returned range
```

---

# 46. Performance Model

The intended performance characteristics are:

```text
Comparison
    |
    v
O(1)
```

Lookup:

```text
Binary Search
    |
    v
O(log n)
```

Insertion:

```text
Sorted insertion
    |
    v
Potentially O(n)
```

Dynamic growth:

```text
std::vector reallocation
    |
    v
Potentially O(n) for a reallocation event
```

The exact measured performance depends on compiler,
optimization level, target hardware, data size and implementation.

---

# 47. Storage Data Lifecycle

A single Entry follows this lifecycle:

```text
Input
 |
 v
Parse
 |
 v
Validate
 |
 v
Create Entry
 |
 v
Insert
 |
 v
Sort / Maintain invariant
 |
 v
Store in RAM
 |
 +-------> Get
 |
 +-------> Range Query
 |
 v
Process exits
 |
 v
RAM storage destroyed
```

Unless persistent storage has been implemented, the lifecycle
ends when the process terminates.

---

# 48. Interview Explanation

A concise explanation is:

> "The application uses a date-indexed in-memory storage model. Input is parsed by `main.cpp`, passed through `BoxManager`, converted into an `Entry` containing a validated `Date` and payload, and stored in either `StaticBox` or `DynamicBox`. The entries maintain chronological ordering, allowing binary-search-based lookup and efficient range queries. StaticBox provides fixed-capacity storage while DynamicBox uses a vector for dynamic growth. SD/eMMC is used for the Linux image and application files; it should not be confused with the runtime Entry storage unless persistence is explicitly implemented."

---

# 49. Final Storage Architecture

```text
             BeagleBone Black
                   |
        +----------+----------+
        |                     |
        v                     v
   SD / eMMC                RAM
        |                     |
        v                     v
   Linux RootFS          Application
        |                     |
        v                     v
      boxes              BoxManager
                              |
                    +---------+---------+
                    |                   |
                    v                   v
                StaticBox          DynamicBox
                    |                   |
                    +---------+---------+
                              |
                              v
                            Entry
                              |
                       +------+------+
                       |             |
                       v             v
                      Date        Payload
                             
                              |
                              v
                       Sorted Storage
                              |
                    +---------+---------+
                    |                   |
                    v                   v
                 Get/Search         Range Query
                    |                   |
                    +---------+---------+
                              |
                              v
                           OUTPUT
```

This gives you the full **data-storage story** for the project: **where the executable lives on SD/eMMC, where the runtime data lives in RAM, how an Entry is created, how StaticBox/DynamicBox store it, how dates maintain ordering, and how `get` and range queries reach the final output.**

