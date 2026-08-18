#include "Entry.hpp"

Entry::Entry()
    : date(),
      data()
{
}

Entry::Entry(const Date& date,
             const std::string& data)
    : date(date),
      data(data)
{
}
