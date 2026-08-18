#ifndef BOX_MANAGER_HPP
#define BOX_MANAGER_HPP

#include "Date.hpp"
#include "Entry.hpp"
#include "StaticBox.hpp"
#include "DynamicBox.hpp"

#include <cstddef>
#include <string>
#include <vector>

class BoxManager
{
public:
    enum class StorageType
    {
        STATIC,
        DYNAMIC
    };

public:
    explicit BoxManager(std::size_t static_capacity = 50);

    // Add a date-indexed record
    bool add(StorageType type,
             const Date& date,
             const std::string& data);

    // Find a record for a specific date
    const Entry* get(StorageType type,
                     const Date& date) const;

    // Fetch all records between two dates
    std::vector<Entry> range(StorageType type,
                             const Date& start,
                             const Date& end) const;

    // Number of stored records
    std::size_t size(StorageType type) const;

    // Maximum capacity of selected storage
    std::size_t capacity(StorageType type) const;

    // Check whether storage is empty
    bool empty(StorageType type) const;

    // Remove all records
    void clear(StorageType type);

private:
    StaticBox<50> static_box_;
    DynamicBox dynamic_box_;

    // Validate date range
    bool is_valid_range(const Date& start,
                        const Date& end) const;
};

#endif // BOX_MANAGER_HPP
