# build
make

# run sample demo
./bin/boxes demo

# add entries
./bin/boxes add static 2025-12-01 "Sensor A reading"
./bin/boxes add dynamic 2025-11-30 "Sensor B reading"

# fetch
./bin/boxes get static 2025-12-01
./bin/boxes range dynamic 2025-11-01 2025-12-31


Key design notes

Date is a small struct with year, month, day; supports parsing YYYY-MM-DD and comparisons.

Entry stores a Date and a std::string payload.

StaticBox uses a fixed capacity determined at compile-time (template parameter). When full, further add returns an error.

DynamicBox manages a std::vector<Entry> and grows as needed.

Both boxes provide the same API so they can be used interchangeably in demo/tests.


