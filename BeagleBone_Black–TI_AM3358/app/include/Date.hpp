#ifndef DATE_HPP
#define DATE_HPP

#include <compare>
#include <string>

class Date
{
public:
    Date();
    Date(int year, int month, int day);

    // Create Date from YYYY-MM-DD
    static Date from_string(const std::string& iso);

    // Convert Date to YYYY-MM-DD
    std::string to_string() const;

    // Accessors
    int year() const;
    int month() const;
    int day() const;

    // Validation
    bool is_valid() const;

    // C++20 three-way comparison
    auto operator<=>(const Date& other) const = default;

private:
    int year_;
    int month_;
    int day_;
};

#endif // DATE_HPP
