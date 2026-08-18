#ifndef STATIC_BOX_HPP
#define STATIC_BOX_HPP

#include "Entry.hpp"

#include <array>
#include <cstddef>
#include <string>

template <std::size_t Capacity>
class StaticBox
{
public:
    StaticBox();

    // Add an entry while maintaining date ordering
    bool add(const Date& date,
             const std::string& data);

    // Find entry by date
    const Entry* find(const Date& date) const;

    // Return all entries in date range
    std::size_t range(const Date& start,
                      const Date& end,
                      Entry* output,
                      std::size_t output_capacity) const;

    // Remove all entries
    void clear();

    // Information
    std::size_t size() const;
    constexpr std::size_t capacity() const;
    bool empty() const;
    bool full() const;

private:
    std::array<Entry, Capacity> entries_;
    std::size_t size_;

    // Find insertion position
    std::size_t lower_bound(const Date& date) const;
};

template <std::size_t Capacity>
constexpr std::size_t StaticBox<Capacity>::capacity() const
{
    return Capacity;
}

#endif // STATIC_BOX_HPP
