#include "Date.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

void test_date_construction()
{
    Date date(2026, 8, 18);

    assert(date.year() == 2026);
    assert(date.month() == 8);
    assert(date.day() == 18);

    std::cout << "[PASS] Date construction\n";
}

void test_date_to_string()
{
    Date date(2026, 8, 18);

    assert(date.to_string() == "2026-08-18");

    Date date2(2025, 1, 5);

    assert(date2.to_string() == "2025-01-05");

    std::cout << "[PASS] Date to_string\n";
}

void test_date_from_string()
{
    Date date =
        Date::from_string("2026-08-18");

    assert(date.year() == 2026);
    assert(date.month() == 8);
    assert(date.day() == 18);

    std::cout << "[PASS] Date from_string\n";
}

void test_valid_dates()
{
    assert(Date(2026, 8, 18).is_valid());
    assert(Date(2025, 1, 1).is_valid());
    assert(Date(2024, 2, 29).is_valid());

    std::cout << "[PASS] Valid dates\n";
}

void test_invalid_dates()
{
    assert(!Date(2026, 0, 10).is_valid());
    assert(!Date(2026, 13, 10).is_valid());
    assert(!Date(2026, 4, 31).is_valid());
    assert(!Date(2026, 2, 29).is_valid());

    std::cout << "[PASS] Invalid dates\n";
}

void test_leap_year()
{
    assert(Date(2024, 2, 29).is_valid());
    assert(!Date(2025, 2, 29).is_valid());
    assert(Date(2000, 2, 29).is_valid());
    assert(!Date(1900, 2, 29).is_valid());

    std::cout << "[PASS] Leap year validation\n";
}

void test_date_comparison()
{
    Date date1(2026, 8, 18);
    Date date2(2026, 8, 19);
    Date date3(2026, 8, 18);

    assert(date1 < date2);
    assert(date2 > date1);

    assert(date1 == date3);
    assert(date1 != date2);

    assert(date1 <= date2);
    assert(date1 <= date3);

    assert(date2 >= date1);
    assert(date1 >= date3);

    std::cout << "[PASS] Date comparison\n";
}

void test_invalid_string()
{
    bool exception_thrown = false;

    try
    {
        Date::from_string("2026-13-40");
    }
    catch (const std::exception&)
    {
        exception_thrown = true;
    }

    assert(exception_thrown);

    std::cout << "[PASS] Invalid date string\n";
}

void test_invalid_format()
{
    bool exception_thrown = false;

    try
    {
        Date::from_string("18-08-2026");
    }
    catch (const std::exception&)
    {
        exception_thrown = true;
    }

    assert(exception_thrown);

    std::cout << "[PASS] Invalid date format\n";
}

int main()
{
    std::cout
        << "\n"
        << "============================\n"
        << "      Date Unit Tests\n"
        << "============================\n\n";

    test_date_construction();
    test_date_to_string();
    test_date_from_string();
    test_valid_dates();
    test_invalid_dates();
    test_leap_year();
    test_date_comparison();
    test_invalid_string();
    test_invalid_format();

    std::cout
        << "\n"
        << "============================\n"
        << "All Date tests PASSED\n"
        << "============================\n";

    return 0;
}
