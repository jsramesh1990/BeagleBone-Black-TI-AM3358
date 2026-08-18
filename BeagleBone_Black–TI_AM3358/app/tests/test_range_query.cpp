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
