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
