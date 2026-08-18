#pragma once
#include "date.h"
#include <string>
#include <vector>
#include <array>
#include <optional>


struct Entry {
Date date;
std::string payload;
};


// Common interface (not formalized via virtual base to keep simple)


// StaticBox: fixed-capacity container
template <size_t CAP>
class StaticBox {
public:
StaticBox();
bool add(const Entry &e, std::string &err);
std::vector<Entry> get_by_date(const Date &d) const;
std::vector<Entry> get_range(const Date &from, const Date &to) const;
size_t size() const { return count; }
size_t capacity() const { return CAP; }
private:
std::array<Entry, CAP> buf;
size_t count;
};


// DynamicBox: resizable container
class DynamicBox {
public:
DynamicBox();
bool add(const Entry &e, std::string &err);
std::vector<Entry> get_by_date(const Date &d) const;
std::vector<Entry> get_range(const Date &from, const Date &to) const;
size_t size() const { return items.size(); }
private:
std::vector<Entry> items;
};


// Utility functions used by both
void sort_entries(std::vector<Entry> &v);
