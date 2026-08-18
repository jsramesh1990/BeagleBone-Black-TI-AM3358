#pragma once
#include <string>
#include <tuple>


struct Date {
int y, m, d;
Date(): y(0), m(0), d(0) {}
Date(int yy,int mm,int dd): y(yy), m(mm), d(dd) {}
static bool parse(const std::string &s, Date &out); // YYYY-MM-DD
std::string to_string() const;
std::tuple<int,int,int> as_tuple() const { return {y,m,d}; }
};

inline bool operator<(const Date &a, const Date &b) { return a.as_tuple() < b.as_tuple(); }
inline bool operator==(const Date &a, const Date &b) { return a.as_tuple() == b.as_tuple(); }
inline bool operator<=(const Date &a, const Date &b) { return !(b < a); }
inline bool operator>=(const Date &a, const Date &b) { return !(a < b); }

