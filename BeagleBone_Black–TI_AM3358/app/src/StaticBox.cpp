#include "StaticBox.hpp"

#include <algorithm>

template <std::size_t Capacity>
StaticBox<Capacity>::StaticBox()
    : entries_{},
      size_(0)
{
}

template <std::size_t Capacity>
bool StaticBox<Capacity>::add(const Date& date,
                              const std::string& data)
{
    // Check whether the box is already full.
    if (full())
    {
        return false;
    }

    // Find the correct insertion position.
    const std::size_t position = lower_bound(date);

    // Do not allow duplicate dates.
    if (position < size_ &&
        entries_[position].date == date)
    {
        return false;
    }

    /*
     * Move existing entries one position to the right.
     *
     * Example:
     *
     * Before:
     * 2026-08-15
     * 2026-08-17
     *
     * Add:
     * 2026-08-16
     *
     * After:
     * 2026-08-15
     * 2026-08-16
     * 2026-08-17
     */
    for (std::size_t i = size_; i > position; --i)
    {
        entries_[i] = entries_[i - 1];
    }

    // Insert the new entry.
    entries_[position] = Entry(date, data);

    ++size_;

    return true;
}

template <std::size_t Capacity>
const Entry* StaticBox<Capacity>::find(const Date& date) const
{
    const std::size_t position = lower_bound(date);

    if (position < size_ &&
        entries_[position].date == date)
    {
        return &entries_[position];
    }

    return nullptr;
}

template <std::size_t Capacity>
std::size_t StaticBox<Capacity>::range(
    const Date& start,
    const Date& end,
    Entry* output,
    std::size_t output_capacity) const
{
    if (output == nullptr ||
        output_capacity == 0 ||
        end < start)
    {
        return 0;
    }

    const std::size_t start_position =
        lower_bound(start);

    std::size_t count = 0;

    for (std::size_t i = start_position;
         i < size_;
         ++i)
    {
        // Stop once entries are beyond the requested range.
        if (entries_[i].date > end)
        {
            break;
        }

        // Prevent output buffer overflow.
        if (count >= output_capacity)
        {
            break;
        }

        output[count] = entries_[i];
        ++count;
    }

    return count;
}

template <std::size_t Capacity>
void StaticBox<Capacity>::clear()
{
    size_ = 0;
}

template <std::size_t Capacity>
std::size_t StaticBox<Capacity>::size() const
{
    return size_;
}

template <std::size_t Capacity>
constexpr std::size_t StaticBox<Capacity>::capacity() const
{
    return Capacity;
}

template <std::size_t Capacity>
bool StaticBox<Capacity>::empty() const
{
    return size_ == 0;
}

template <std::size_t Capacity>
bool StaticBox<Capacity>::full() const
{
    return size_ >= Capacity;
}

template <std::size_t Capacity>
std::size_t StaticBox<Capacity>::lower_bound(
    const Date& date) const
{
    std::size_t left = 0;
    std::size_t right = size_;

    /*
     * Binary search.
     *
     * We find the first position where:
     *
     * entries_[position].date >= date
     */
    while (left < right)
    {
        const std::size_t middle =
            left + (right - left) / 2;

        if (entries_[middle].date < date)
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    return left;
}


/*
 * Explicit template instantiation.
 *
 * BoxManager uses:
 *
 *     StaticBox<50>
 *
 * Therefore we explicitly generate the implementation
 * for that particular capacity.
 */
template class StaticBox<50>;
