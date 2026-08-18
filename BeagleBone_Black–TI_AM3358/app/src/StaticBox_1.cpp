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

    bool add(const Date& date,
             const std::string& data);

    const Entry* find(const Date& date) const;

    std::size_t range(const Date& start,
                      const Date& end,
                      Entry* output,
                      std::size_t output_capacity) const;

    void clear();

    std::size_t size() const;

    constexpr std::size_t capacity() const;

    bool empty() const;

    bool full() const;

private:
    std::array<Entry, Capacity> entries_;

    std::size_t size_;

    std::size_t lower_bound(const Date& date) const;
};

#endif // STATIC_BOX_HPP
