#include "BoxManager.hpp"

bool BoxManager::add(StorageType type,
                     const Date& date,
                     const std::string& data)
{
    switch (type)
    {
        case StorageType::STATIC:
            return static_box_.add(date, data);

        case StorageType::DYNAMIC:
            return dynamic_box_.add(date, data);
    }

    return false;
}

const Entry* BoxManager::get(StorageType type,
                             const Date& date) const
{
    switch (type)
    {
        case StorageType::STATIC:
            return static_box_.find(date);

        case StorageType::DYNAMIC:
            return dynamic_box_.find(date);
    }

    return nullptr;
}

std::vector<Entry> BoxManager::range(StorageType type,
                                     const Date& start,
                                     const Date& end) const
{
    if (!is_valid_range(start, end))
    {
        return {};
    }

    switch (type)
    {
        case StorageType::STATIC:
        {
            /*
             * StaticBox uses a fixed output buffer.
             *
             * The maximum number of records is the
             * static box capacity.
             */
            Entry output[50];

            const std::size_t count =
                static_box_.range(
                    start,
                    end,
                    output,
                    50);

            return std::vector<Entry>(
                output,
                output + count);
        }

        case StorageType::DYNAMIC:
            return dynamic_box_.range(start, end);
    }

    return {};
}

std::size_t BoxManager::size(StorageType type) const
{
    switch (type)
    {
        case StorageType::STATIC:
            return static_box_.size();

        case StorageType::DYNAMIC:
            return dynamic_box_.size();
    }

    return 0;
}

std::size_t BoxManager::capacity(StorageType type) const
{
    switch (type)
    {
        case StorageType::STATIC:
            return static_box_.capacity();

        case StorageType::DYNAMIC:
            return dynamic_box_.capacity();
    }

    return 0;
}

bool BoxManager::empty(StorageType type) const
{
    switch (type)
    {
        case StorageType::STATIC:
            return static_box_.empty();

        case StorageType::DYNAMIC:
            return dynamic_box_.empty();
    }

    return true;
}

void BoxManager::clear(StorageType type)
{
    switch (type)
    {
        case StorageType::STATIC:
            static_box_.clear();
            break;

        case StorageType::DYNAMIC:
            dynamic_box_.clear();
            break;
    }
}

bool BoxManager::is_valid_range(const Date& start,
                                const Date& end) const
{
    return start <= end;
}
