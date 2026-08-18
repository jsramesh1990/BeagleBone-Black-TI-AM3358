#include "box.h"
#include <algorithm>
#include <sstream>


// StaticBox implementation
template <size_t CAP>
StaticBox<CAP>::StaticBox(): count(0) {}


template <size_t CAP>
bool StaticBox<CAP>::add(const Entry &e, std::string &err) {
if (count >= CAP) { err = "StaticBox is full"; return false; }
buf[count++] = e;
return true;
}


template <size_t CAP>
std::vector<Entry> StaticBox<CAP>::get_by_date(const Date &d) const {
std::vector<Entry> out;
for (size_t i=0;i<count;++i) if (buf[i].date == d) out.push_back(buf[i]);
sort_entries(out);
return out;
}


template <size_t CAP>
std::vector<Entry> StaticBox<CAP>::get_range(const Date &from, const Date &to) const {
std::vector<Entry> out;
for (size_t i=0;i<count;++i) if (buf[i].date >= from && buf[i].date <= to) out.push_back(buf[i]);
sort_entries(out);
return out;
}


// Force instantiation for a default size used in main
template class StaticBox<100>;


// DynamicBox implementation
DynamicBox::DynamicBox() { items.reserve(8); }


bool DynamicBox::add(const Entry &e, std::string &err) {
items.push_back(e);
return true;
}


std::vector<Entry> DynamicBox::get_by_date(const Date &d) const {
std::vector<Entry> out;
for (const auto &it: items) if (it.date == d) out.push_back(it);
auto copy = out; sort_entries(copy);
return copy;
}


std::vector<Entry> DynamicBox::get_range(const Date &from, const Date &to) const {
std::vector<Entry> out;
for (const auto &it: items) if (it.date >= from && it.date <= to) out.push_back(it);
sort_entries(out);
return out;
}


void sort_entries(std::vector<Entry> &v) {
std::sort(v.begin(), v.end(), [](const Entry &a, const Entry &b){ return a.date < b.date; });
}


// Note: because StaticBox is a template, the implementation was included here and we forced
// an instantiation for a reasonable default used by main (CAP=100).
