#include "date.h"
#include <sstream>
#include <iomanip>

bool Date::parse(const std::string &s, Date &out) {
    // Expect format YYYY-MM-DD
    if (s.size() != 10) return false;
    int yy, mm, dd;
    char c1,c2;
    std::istringstream iss(s);
    if (!(iss >> yy >> c1 >> mm >> c2 >> dd)) return false;
    if (c1!='-'||c2!='-') return false;
    // basic validation
    if (mm < 1 || mm > 12) return false;
    if (dd < 1 || dd > 31) return false;
    out = Date(yy,mm,dd);
    return true;
}

std::string Date::to_string() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << y << '-' << std::setw(2) << m << '-' << std::setw(2) << d;
    return oss.str();
}
