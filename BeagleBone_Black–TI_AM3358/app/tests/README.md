Build the Date test

From the project root:

g++ -std=c++17 -Wall -Wextra -O2 \
    -Iapp/include \
    app/src/Date.cpp \
    app/tests/test_date.cpp \
    -o test_date

Run:

./test_date

Expected:

============================
      Date Unit Tests
============================


[PASS] Date construction
[PASS] Date to_string
[PASS] Date from_string
[PASS] Valid dates
[PASS] Invalid dates
[PASS] Leap year validation
[PASS] Date comparison
[PASS] Invalid date string
[PASS] Invalid date format


============================
All Date tests PASSED
============================
Then we expand the tests

After test_date.cpp, I recommend adding:

tests/
├── test_date.cpp
├── test_entry.cpp
├── test_static_box.cpp
├── test_dynamic_box.cpp
└── test_box_manager.cpp

The testing flow will then be:

test_date
    ↓
Date works
    ↓
test_entry
    ↓
Entry works
    ↓
test_static_box
    ↓
StaticBox works
    ↓
test_dynamic_box
    ↓
DynamicBox works
    ↓
test_box_manager
    ↓
Complete application logic verified
    ↓
Build ARM binary
    ↓
Deploy to BeagleBone Black

One important point: before compiling this test, your Date.hpp and Date.cpp must use the C++17 comparison functions (<, ==, !=, >, <=, >=) we defined earlier—not operator<=>.



Yes. These are the next three unit-test files. We can make them test the actual behavior of your **StaticBox**, **DynamicBox**, and date-range retrieval.

Your test directory becomes:

```text
app/
└── tests/
    ├── test_date.cpp
    ├── test_dynamic_box.cpp
    ├── test_range_query.cpp
    └── test_static_box.cpp
```

---

# 1. `test_static_box.cpp`

This tests:

* Adding entries
* Sorted ordering
* Duplicate-date rejection
* Capacity handling
* Date lookup
* Clear
* Empty/full state

```cpp
#include "StaticBox.hpp"

#include <cassert>
#include <iostream>

void test_add()
{
    StaticBox<5> box;

    bool result = box.add(
        Date(2026, 8, 18),
        "Temperature=32C");

    assert(result);
    assert(box.size() == 1);

    std::cout << "[PASS] StaticBox add\n";
}

void test_sorted_insertion()
{
    StaticBox<5> box;

    box.add(
        Date(2026, 8, 18),
        "Data-18");

    box.add(
        Date(2026, 8, 15),
        "Data-15");

    box.add(
        Date(2026, 8, 17),
        "Data-17");

    const Entry* first =
        box.find(Date(2026, 8, 15));

    const Entry* second =
        box.find(Date(2026, 8, 17));

    const Entry* third =
        box.find(Date(2026, 8, 18));

    assert(first != nullptr);
    assert(second != nullptr);
    assert(third != nullptr);

    assert(first->data == "Data-15");
    assert(second->data == "Data-17");
    assert(third->data == "Data-18");

    std::cout
        << "[PASS] StaticBox sorted insertion\n";
}

void test_find()
{
    StaticBox<5> box;

    box.add(
        Date(2026, 8, 18),
        "Sensor data");

    const Entry* entry =
        box.find(Date(2026, 8, 18));

    assert(entry != nullptr);
    assert(entry->data == "Sensor data");

    const Entry* missing =
        box.find(Date(2026, 8, 19));

    assert(missing == nullptr);

    std::cout << "[PASS] StaticBox find\n";
}

void test_duplicate_date()
{
    StaticBox<5> box;

    bool first =
        box.add(
            Date(2026, 8, 18),
            "First");

    bool second =
        box.add(
            Date(2026, 8, 18),
            "Second");

    assert(first);
    assert(!second);
    assert(box.size() == 1);

    std::cout
        << "[PASS] StaticBox duplicate date\n";
}

void test_capacity()
{
    StaticBox<3> box;

    assert(box.capacity() == 3);
    assert(box.empty());
    assert(!box.full());

    assert(box.add(
        Date(2026, 8, 15),
        "Data-15"));

    assert(box.add(
        Date(2026, 8, 16),
        "Data-16"));

    assert(box.add(
        Date(2026, 8, 17),
        "Data-17"));

    assert(box.size() == 3);
    assert(box.full());

    bool result =
        box.add(
            Date(2026, 8, 18),
            "Data-18");

    assert(!result);
    assert(box.size() == 3);

    std::cout
        << "[PASS] StaticBox capacity\n";
}

void test_clear()
{
    StaticBox<5> box;

    box.add(
        Date(2026, 8, 18),
        "Data");

    assert(!box.empty());

    box.clear();

    assert(box.empty());
    assert(box.size() == 0);

    const Entry* entry =
        box.find(Date(2026, 8, 18));

    assert(entry == nullptr);

    std::cout << "[PASS] StaticBox clear\n";
}

int main()
{
    std::cout
        << "\n"
        << "============================\n"
        << "     StaticBox Tests\n"
        << "============================\n\n";

    test_add();
    test_sorted_insertion();
    test_find();
    test_duplicate_date();
    test_capacity();
    test_clear();

    std::cout
        << "\n"
        << "============================\n"
        << "All StaticBox tests PASSED\n"
        << "============================\n";

    return 0;
}
```

---

# 2. `test_dynamic_box.cpp`

Now test the `std::vector`-based implementation.

```cpp
#include "DynamicBox.hpp"

#include <cassert>
#include <iostream>

void test_add()
{
    DynamicBox box;

    bool result =
        box.add(
            Date(2026, 8, 18),
            "Temperature=32C");

    assert(result);
    assert(box.size() == 1);
    assert(!box.empty());

    std::cout << "[PASS] DynamicBox add\n";
}

void test_dynamic_resize()
{
    DynamicBox box;

    for (int day = 1; day <= 100; ++day)
    {
        bool result =
            box.add(
                Date(2026, 1, day),
                "Sensor data");

        /*
         * This test intentionally uses dates only
         * up to day 28 to avoid invalid calendar dates.
         */
        if (day > 28)
        {
            break;
        }

        assert(result);
    }

    assert(box.size() == 28);

    std::cout
        << "[PASS] DynamicBox resize\n";
}

void test_sorted_insertion()
{
    DynamicBox box;

    box.add(
        Date(2026, 8, 18),
        "Data-18");

    box.add(
        Date(2026, 8, 15),
        "Data-15");

    box.add(
        Date(2026, 8, 17),
        "Data-17");

    const Entry* first =
        box.find(Date(2026, 8, 15));

    const Entry* second =
        box.find(Date(2026, 8, 17));

    const Entry* third =
        box.find(Date(2026, 8, 18));

    assert(first != nullptr);
    assert(second != nullptr);
    assert(third != nullptr);

    assert(first->data == "Data-15");
    assert(second->data == "Data-17");
    assert(third->data == "Data-18");

    std::cout
        << "[PASS] DynamicBox sorted insertion\n";
}

void test_find()
{
    DynamicBox box;

    box.add(
        Date(2026, 8, 18),
        "Sensor data");

    const Entry* entry =
        box.find(Date(2026, 8, 18));

    assert(entry != nullptr);
    assert(entry->data == "Sensor data");

    const Entry* missing =
        box.find(Date(2026, 8, 19));

    assert(missing == nullptr);

    std::cout << "[PASS] DynamicBox find\n";
}

void test_duplicate_date()
{
    DynamicBox box;

    bool first =
        box.add(
            Date(2026, 8, 18),
            "First");

    bool second =
        box.add(
            Date(2026, 8, 18),
            "Second");

    assert(first);
    assert(!second);

    assert(box.size() == 1);

    std::cout
        << "[PASS] DynamicBox duplicate date\n";
}

void test_clear()
{
    DynamicBox box;

    box.add(
        Date(2026, 8, 18),
        "Data");

    assert(!box.empty());

    box.clear();

    assert(box.empty());
    assert(box.size() == 0);

    const Entry* entry =
        box.find(Date(2026, 8, 18));

    assert(entry == nullptr);

    std::cout << "[PASS] DynamicBox clear\n";
}

int main()
{
    std::cout
        << "\n"
        << "============================\n"
        << "     DynamicBox Tests\n"
        << "============================\n\n";

    test_add();
    test_dynamic_resize();
    test_sorted_insertion();
    test_find();
    test_duplicate_date();
    test_clear();

    std::cout
        << "\n"
        << "============================\n"
        << "All DynamicBox tests PASSED\n"
        << "============================\n";

    return 0;
}
```

### Small correction

The `test_dynamic_resize()` above should use valid dates only. A better version is to test vector growth using several months:

```cpp
void test_dynamic_resize()
{
    DynamicBox box;

    for (int day = 1; day <= 28; ++day)
    {
        bool result =
            box.add(
                Date(2026, 1, day),
                "January data");

        assert(result);
    }

    for (int day = 1; day <= 28; ++day)
    {
        bool result =
            box.add(
                Date(2026, 2, day),
                "February data");

        assert(result);
    }

    assert(box.size() == 56);

    std::cout
        << "[PASS] DynamicBox resize\n";
}
```

**Use this corrected version** in the final file.

---

# 3. `test_range_query.cpp`

This is particularly important because **range retrieval is one of the main features of the project**.

We test:

```text
Start Date
     |
     v
2026-08-15
     |
     |
     v
2026-08-18
     |
     v
End Date
```

Expected records:

```text
15
16
17
18
```

### File

```cpp
#include "DynamicBox.hpp"
#include "StaticBox.hpp"

#include <cassert>
#include <iostream>
#include <vector>

void test_dynamic_range()
{
    DynamicBox box;

    box.add(
        Date(2026, 8, 10),
        "Data-10");

    box.add(
        Date(2026, 8, 15),
        "Data-15");

    box.add(
        Date(2026, 8, 16),
        "Data-16");

    box.add(
        Date(2026, 8, 17),
        "Data-17");

    box.add(
        Date(2026, 8, 18),
        "Data-18");

    box.add(
        Date(2026, 8, 20),
        "Data-20");

    std::vector<Entry> result =
        box.range(
            Date(2026, 8, 15),
            Date(2026, 8, 18));

    assert(result.size() == 4);

    assert(result[0].date ==
           Date(2026, 8, 15));

    assert(result[1].date ==
           Date(2026, 8, 16));

    assert(result[2].date ==
           Date(2026, 8, 17));

    assert(result[3].date ==
           Date(2026, 8, 18));

    std::cout
        << "[PASS] DynamicBox range query\n";
}

void test_dynamic_range_empty()
{
    DynamicBox box;

    box.add(
        Date(2026, 8, 10),
        "Data-10");

    box.add(
        Date(2026, 8, 20),
        "Data-20");

    std::vector<Entry> result =
        box.range(
            Date(2026, 8, 12),
            Date(2026, 8, 15));

    assert(result.empty());

    std::cout
        << "[PASS] DynamicBox empty range\n";
}

void test_dynamic_invalid_range()
{
    DynamicBox box;

    box.add(
        Date(2026, 8, 15),
        "Data-15");

    box.add(
        Date(2026, 8, 20),
        "Data-20");

    std::vector<Entry> result =
        box.range(
            Date(2026, 8, 20),
            Date(2026, 8, 15));

    assert(result.empty());

    std::cout
        << "[PASS] DynamicBox invalid range\n";
}

void test_static_range()
{
    StaticBox<10> box;

    box.add(
        Date(2026, 8, 10),
        "Data-10");

    box.add(
        Date(2026, 8, 15),
        "Data-15");

    box.add(
        Date(2026, 8, 16),
        "Data-16");

    box.add(
        Date(2026, 8, 17),
        "Data-17");

    box.add(
        Date(2026, 8, 18),
        "Data-18");

    box.add(
        Date(2026, 8, 20),
        "Data-20");

    Entry output[10];

    const std::size_t count =
        box.range(
            Date(2026, 8, 15),
            Date(2026, 8, 18),
            output,
            10);

    assert(count == 4);

    assert(output[0].date ==
           Date(2026, 8, 15));

    assert(output[1].date ==
           Date(2026, 8, 16));

    assert(output[2].date ==
           Date(2026, 8, 17));

    assert(output[3].date ==
           Date(2026, 8, 18));

    std::cout
        << "[PASS] StaticBox range query\n";
}

void test_static_range_output_limit()
{
    StaticBox<10> box;

    box.add(
        Date(2026, 8, 15),
        "Data-15");

    box.add(
        Date(2026, 8, 16),
        "Data-16");

    box.add(
        Date(2026, 8, 17),
        "Data-17");

    box.add(
        Date(2026, 8, 18),
        "Data-18");

    Entry output[2];

    const std::size_t count =
        box.range(
            Date(2026, 8, 15),
            Date(2026, 8, 18),
            output,
            2);

    assert(count == 2);

    assert(output[0].date ==
           Date(2026, 8, 15));

    assert(output[1].date ==
           Date(2026, 8, 16));

    std::cout
        << "[PASS] StaticBox range output limit\n";
}

int main()
{
    std::cout
        << "\n"
        << "============================\n"
        << "      Range Query Tests\n"
        << "============================\n\n";

    test_dynamic_range();
    test_dynamic_range_empty();
    test_dynamic_invalid_range();

    test_static_range();
    test_static_range_output_limit();

    std::cout
        << "\n"
        << "============================\n"
        << "All Range tests PASSED\n"
        << "============================\n";

    return 0;
}
```

## Your test architecture now

```text
                    tests/
                       |
        +--------------+--------------+
        |              |              |
        v              v              v
 test_date     test_static_box  test_dynamic_box
        |              |              |
        v              +------+-------+
       Date                   |
                              v
                       test_range_query
                              |
                              v
                    Date-indexed retrieval
```

### What we're validating

```text
                    Date
                     |
             +-------+-------+
             |               |
             v               v
        StaticBox        DynamicBox
             |               |
             +-------+-------+
                     |
                     v
               Range Query
                     |
                     v
              Binary Search
                     |
                     v
                 Results
```

And ultimately the complete project evaluation will be:

```text
Unit Tests
    ↓
Date
    ↓
Entry
    ↓
StaticBox
    ↓
DynamicBox
    ↓
Range Query
    ↓
BoxManager
    ↓
CLI
    ↓
ARM Cross Compilation
    ↓
Yocto Integration
    ↓
BeagleBone Black
    ↓
TI AM3358
    ↓
Boot Linux
    ↓
Run Application
    ↓
Evaluate Output
```

**Next, we should create `test_box_manager.cpp`**, because that will test the complete interface that `main.cpp` uses before we move into the `Makefile` and actual build.


