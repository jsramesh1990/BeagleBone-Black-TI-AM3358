#include "BoxManager.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace
{

void print_usage(const char* program)
{
    std::cout
        << "\nBox Storage System\n"
        << "==================\n\n"

        << "Usage:\n"
        << "  " << program
        << " add <static|dynamic> <YYYY-MM-DD> <data>\n"

        << "  " << program
        << " get <static|dynamic> <YYYY-MM-DD>\n"

        << "  " << program
        << " range <static|dynamic> "
        << "<YYYY-MM-DD> <YYYY-MM-DD>\n"

        << "  " << program
        << " list <static|dynamic>\n"

        << "  " << program
        << " clear <static|dynamic>\n"

        << "  " << program
        << " demo\n"

        << "  " << program
        << " help\n\n";
}

bool parse_storage_type(
    const std::string& value,
    BoxManager::StorageType& type)
{
    if (value == "static")
    {
        type = BoxManager::StorageType::STATIC;
        return true;
    }

    if (value == "dynamic")
    {
        type = BoxManager::StorageType::DYNAMIC;
        return true;
    }

    return false;
}

void print_entry(const Entry& entry)
{
    std::cout
        << entry.date.to_string()
        << " : "
        << entry.data
        << '\n';
}

void print_entries(const std::vector<Entry>& entries)
{
    if (entries.empty())
    {
        std::cout << "No entries found.\n";
        return;
    }

    for (const Entry& entry : entries)
    {
        print_entry(entry);
    }
}

void run_demo()
{
    BoxManager manager;

    std::cout
        << "\n=== StaticBox Demo ===\n";

    manager.add(
        BoxManager::StorageType::STATIC,
        Date(2026, 8, 15),
        "Temperature=31C");

    manager.add(
        BoxManager::StorageType::STATIC,
        Date(2026, 8, 17),
        "Temperature=33C");

    manager.add(
        BoxManager::StorageType::STATIC,
        Date(2026, 8, 16),
        "Temperature=32C");

    std::cout << "\nStatic records:\n";

    auto static_records =
        manager.range(
            BoxManager::StorageType::STATIC,
            Date(2026, 8, 15),
            Date(2026, 8, 17));

    print_entries(static_records);

    std::cout
        << "\n=== DynamicBox Demo ===\n";

    manager.add(
        BoxManager::StorageType::DYNAMIC,
        Date(2026, 8, 18),
        "Temperature=34C");

    manager.add(
        BoxManager::StorageType::DYNAMIC,
        Date(2026, 8, 16),
        "Temperature=32C");

    manager.add(
        BoxManager::StorageType::DYNAMIC,
        Date(2026, 8, 17),
        "Temperature=33C");

    std::cout << "\nDynamic records:\n";

    auto dynamic_records =
        manager.range(
            BoxManager::StorageType::DYNAMIC,
            Date(2026, 8, 16),
            Date(2026, 8, 18));

    print_entries(dynamic_records);

    std::cout
        << "\n=== Demo Complete ===\n";
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string command = argv[1];

    if (command == "help")
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (command == "demo")
    {
        run_demo();
        return EXIT_SUCCESS;
    }

    BoxManager manager;

    if (command == "add")
    {
        if (argc < 5)
        {
            std::cerr
                << "Error: add requires "
                << "<static|dynamic> <date> <data>\n";

            return EXIT_FAILURE;
        }

        BoxManager::StorageType type;

        if (!parse_storage_type(argv[2], type))
        {
            std::cerr
                << "Error: invalid storage type: "
                << argv[2] << '\n';

            return EXIT_FAILURE;
        }

        try
        {
            const Date date =
                Date::from_string(argv[3]);

            const std::string data = argv[4];

            if (manager.add(type, date, data))
            {
                std::cout
                    << "Entry added successfully.\n";

                return EXIT_SUCCESS;
            }

            std::cerr
                << "Error: could not add entry.\n"
                << "The date may already exist or "
                << "StaticBox may be full.\n";

            return EXIT_FAILURE;
        }
        catch (const std::exception& error)
        {
            std::cerr
                << "Date error: "
                << error.what()
                << '\n';

            return EXIT_FAILURE;
        }
    }

    if (command == "get")
    {
        if (argc < 4)
        {
            std::cerr
                << "Error: get requires "
                << "<static|dynamic> <date>\n";

            return EXIT_FAILURE;
        }

        BoxManager::StorageType type;

        if (!parse_storage_type(argv[2], type))
        {
            std::cerr
                << "Error: invalid storage type: "
                << argv[2] << '\n';

            return EXIT_FAILURE;
        }

        try
        {
            const Date date =
                Date::from_string(argv[3]);

            const Entry* entry =
                manager.get(type, date);

            if (entry != nullptr)
            {
                print_entry(*entry);
                return EXIT_SUCCESS;
            }

            std::cout
                << "Entry not found.\n";

            return EXIT_FAILURE;
        }
        catch (const std::exception& error)
        {
            std::cerr
                << "Date error: "
                << error.what()
                << '\n';

            return EXIT_FAILURE;
        }
    }

    if (command == "range")
    {
        if (argc < 5)
        {
            std::cerr
                << "Error: range requires "
                << "<static|dynamic> "
                << "<start-date> <end-date>\n";

            return EXIT_FAILURE;
        }

        BoxManager::StorageType type;

        if (!parse_storage_type(argv[2], type))
        {
            std::cerr
                << "Error: invalid storage type: "
                << argv[2] << '\n';

            return EXIT_FAILURE;
        }

        try
        {
            const Date start =
                Date::from_string(argv[3]);

            const Date end =
                Date::from_string(argv[4]);

            const std::vector<Entry> entries =
                manager.range(type, start, end);

            print_entries(entries);

            return EXIT_SUCCESS;
        }
        catch (const std::exception& error)
        {
            std::cerr
                << "Date error: "
                << error.what()
                << '\n';

            return EXIT_FAILURE;
        }
    }

    if (command == "list")
    {
        if (argc < 3)
        {
            std::cerr
                << "Error: list requires "
                << "<static|dynamic>\n";

            return EXIT_FAILURE;
        }

        BoxManager::StorageType type;

        if (!parse_storage_type(argv[2], type))
        {
            std::cerr
                << "Error: invalid storage type: "
                << argv[2] << '\n';

            return EXIT_FAILURE;
        }

        std::cout
            << "Records: "
            << manager.size(type)
            << '\n';

        std::cout
            << "Capacity: "
            << manager.capacity(type)
            << '\n';

        std::cout
            << "Empty: "
            << (manager.empty(type) ? "yes" : "no")
            << '\n';

        return EXIT_SUCCESS;
    }

    if (command == "clear")
    {
        if (argc < 3)
        {
            std::cerr
                << "Error: clear requires "
                << "<static|dynamic>\n";

            return EXIT_FAILURE;
        }

        BoxManager::StorageType type;

        if (!parse_storage_type(argv[2], type))
        {
            std::cerr
                << "Error: invalid storage type: "
                << argv[2] << '\n';

            return EXIT_FAILURE;
        }

        manager.clear(type);

        std::cout
            << "Storage cleared successfully.\n";

        return EXIT_SUCCESS;
    }

    std::cerr
        << "Error: unknown command: "
        << command
        << "\n";

    print_usage(argv[0]);

    return EXIT_FAILURE;
}
