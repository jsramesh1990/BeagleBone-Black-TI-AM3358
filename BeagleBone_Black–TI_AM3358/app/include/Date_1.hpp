#ifndef DATE_HPP
#define DATE_HPP

#include <string>

class Date
{
public:
    Date();
    Date(int year, int month, int day);

    static Date from_string(const std::string& iso);

    std::string to_string() const;

    int year() const;
    int month() const;
    int day() const;

    bool is_valid() const;

    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

private:
    int year_;
    int month_;
    int day_;
};

#endif // DATE_HPP
