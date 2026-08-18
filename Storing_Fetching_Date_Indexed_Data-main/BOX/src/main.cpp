#include <iostream>
#include <string>
#include <vector>
#include "date.h"
#include "box.h"


using namespace std;


// Use the default StaticBox size of 100
using MyStatic = StaticBox<100>;


void print_entries(const vector<Entry> &v) {
for (const auto &e: v) cout << e.date.to_string() << " : " << e.payload << "\n";
}


int cmd_demo(int argc, char**argv) {
MyStatic sbox;
DynamicBox dbox;
string err;
Entry e1{Date(2025,12,1), "Sensor A: 12.5"};
Entry e2{Date(2025,12,1), "Sensor B: 11.2"};
Entry e3{Date(2025,11,30), "Sensor C: 9.0"};
sbox.add(e1, err); sbox.add(e2, err); sbox.add(e3, err);
dbox.add(e1, err); dbox.add(e2, err); dbox.add(e3, err);
cout << "StaticBox contents:\n"; print_entries(sbox.get_range(Date(2025,11,1), Date(2025,12,31)));
cout << "\nDynamicBox contents:\n"; print_entries(dbox.get_range(Date(2025,11,1), Date(2025,12,31)));
return 0;
}


int cmd_add(int argc, char**argv) {
if (argc < 5) { cerr << "Usage: add <static|dynamic> <YYYY-MM-DD> \"payload\"\n"; return 1; }
string which = argv[2];
Date d; if (!Date::parse(argv[3], d)) { cerr << "Invalid date\n"; return 1; }
string payload = argv[4];
// For demo simplicity we create local boxes and add then print — in a real project you'd persist.
MyStatic sbox; DynamicBox dbox; string err;
if (which == "static") { if (!sbox.add({d,payload}, err)) cerr << "Error: " << err << '\n'; else cout << "Added to static\n"; }
else { if (!dbox.add({d,payload}, err)) cerr << "Error: " << err << '\n'; else cout << "Added to dynamic\n"; }
return 0;
}


int cmd_get(int argc, char**argv) {
if (argc < 4) { cerr << "Usage: get <static|dynamic> <YYYY-MM-DD>\n"; return 1; }
string which = argv[2]; Date d; if (!Date::parse(argv[3], d)) { cerr << "Invalid date\n"; return 1; }
// Demo: prefill a box and query
MyStatic sbox; DynamicBox dbox; string err;
sbox.add({Date(2025,12,1), "Sensor A"}, err);
dbox.add({Date(2025,12,1), "Sensor A"}, err);
if (which == "static") print_entries(sbox.get_by_date(d)); else print_entries(dbox.get_by_date(d));
return 0;
}


int main(int argc, char** argv) {
if (argc < 2) { cerr << "Usage: boxes <command> [args]\nCommands: demo, add, get, range\n"; return 1; }
string cmd = argv[1];
if (cmd == "demo") return cmd_demo(argc, argv);
if (cmd == "add") return cmd_add(argc, argv);
if (cmd == "get") return cmd_get(argc, argv);
if (cmd == "range") { cerr << "Range command not implemented in CLI demo\n"; return 0; }
cerr << "Unknown command" << endl; return 1;
}
