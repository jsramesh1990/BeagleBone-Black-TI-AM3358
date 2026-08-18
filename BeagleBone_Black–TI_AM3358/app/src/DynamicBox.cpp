#include "DynamicBox.hpp"

#include <algorithm>

bool DynamicBox::add(const Date& date,
                     const std::string& data)
{
    // Find the position where this date should be inserted.
    const std::size_t position = lower_bound(date);

    // Do not allow duplicate dates.
    if (position < entries_.size() &&
        entries_[position].date == date)
    {
        return false;
    }

    Entry entry(date, data);

    // Insert while maintaining sorted date order.
    entries_.insert(entries_.begin() + position, entry);

    return true;
}

const Entry* DynamicBox::find(const Date& date) const
{
    const std::size_t position = lower_bound(date);

    if (position < entries_.size() &&
        entries_[position].date == date)
    {
        return &entries_[position];
    }

    return nullptr;
}

std::vector<Entry> DynamicBox::range(const Date& start,
                                     const Date& end) const
{
    std::vector<Entry> result;

    if (end < start)
    {
        return result;
    }

    const std::size_t start_position = lower_bound(start);

    for (std::size_t i = start_position;
         i < entries_.size();
         ++i)
    {
        if (entries_[i].date > end)
        {
            break;
        }

        result.push_back(entries_[i]);
    }

    return result;
}

void DynamicBox::clear()
{
    entries_.clear();
}

std::size_t DynamicBox::size() const
{
    return entries_.size();
}

std::size_t DynamicBox::capacity() const
{
    return entries_.capacity();
}

bool DynamicBox::empty() const
{
    return entries_.empty();
}

std::size_t DynamicBox::lower_bound(const Date& date) const
{
    std::size_t left = 0;
    std::size_t right = entries_.size();

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
