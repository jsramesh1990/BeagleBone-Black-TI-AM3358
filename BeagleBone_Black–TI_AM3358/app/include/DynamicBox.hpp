#ifndef DYNAMIC_BOX_HPP
#define DYNAMIC_BOX_HPP

#include "Entry.hpp"

#include <cstddef>
#include <string>
#include <vector>

class DynamicBox
{
public:
    DynamicBox() = default;

    // Add an entry while maintaining date ordering
    bool add(const Date& date,
             const std::string& data);

    // Find entry by date
    const Entry* find(const Date& date) const;

    // Return all entries in date range
    std::vector<Entry> range(const Date& start,
                             const Date& end) const;

    // Remove all entries
    void clear();

    // Information
    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;

private:
    std::vector<Entry> entries_;

    // Find insertion position
    std::size_t lower_bound(const Date& date) const;
};

#endif // DYNAMIC_BOX_HPP
