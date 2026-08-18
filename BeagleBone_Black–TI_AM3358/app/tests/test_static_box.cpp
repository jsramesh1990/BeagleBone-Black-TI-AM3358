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
