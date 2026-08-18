#ifndef ENTRY_HPP
#define ENTRY_HPP

#include "Date.hpp"

#include <string>

struct Entry
{
    Date date;
    std::string data;

    Entry() = default;

    Entry(const Date& date,
          const std::string& data)
        : date(date),
          data(data)
    {
    }
};

#endif // ENTRY_HPP
