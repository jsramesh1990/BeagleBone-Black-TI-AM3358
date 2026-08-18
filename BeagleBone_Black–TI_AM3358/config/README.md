hese are runtime configuration files, not C++ source files.

1. config/boxes.conf

This controls the behavior of the Box Storage application.

# ============================================================
# Box Storage System Configuration
# BeagleBone Black - TI AM3358
# ============================================================


# Default storage type
# Options:
#   static
#   dynamic


storage_type=dynamic




# StaticBox capacity
# This value is used when the static storage mode is selected.


static_capacity=50




# Enable duplicate date protection
# 1 = enabled
# 0 = disabled


reject_duplicate_dates=1




# Date format
# Currently supported format:


date_format=YYYY-MM-DD




# Enable range queries
# 1 = enabled
# 0 = disabled


enable_range_query=1




# Maximum number of records returned by a range query


max_range_results=100




# Data storage path


data_directory=/var/lib/boxes




# Persistent data file


data_file=/var/lib/boxes/boxes.data
What this means

For example:

storage_type=dynamic

means the application uses:

                  BoxManager
                      |
                      v
                DynamicBox
                      |
                      v
                 std::vector

If changed to:

storage_type=static

the application uses:

                  BoxManager
                      |
                      v
                 StaticBox
                      |
                      v
                std::array
2. config/logging.conf

This controls application logging.

# ============================================================
# Box Storage Logging Configuration
# BeagleBone Black - TI AM3358
# ============================================================


# Logging level
#
# Options:
#   DEBUG
#   INFO
#   WARN
#   ERROR


level=INFO




# Enable console logging
# 1 = enabled
# 0 = disabled


console=1




# Enable file logging
# 1 = enabled
# 0 = disabled


file=1




# Log file location


log_file=/var/log/boxes/boxes.log




# Maximum log file size in KB


max_file_size=1024




# Number of backup log files


backup_count=3




# Include timestamp


timestamp=1




# Include log level


include_level=1
How these two files fit into the project

The overall structure now becomes:

BeagleBone_Black-TI_AM3358/
│
├── app/
│   │
│   ├── include/
│   │   ├── Date.hpp
│   │   ├── Entry.hpp
│   │   ├── StaticBox.hpp
│   │   ├── DynamicBox.hpp
│   │   └── BoxManager.hpp
│   │
│   ├── src/
│   │   ├── Date.cpp
│   │   ├── Entry.cpp
│   │   ├── StaticBox.cpp
│   │   ├── DynamicBox.cpp
│   │   ├── BoxManager.cpp
│   │   └── main.cpp
│   │
│   └── tests/
│       ├── test_date.cpp
│       ├── test_static_box.cpp
│       ├── test_dynamic_box.cpp
│       └── test_range_query.cpp
│
├── config/
│   ├── boxes.conf
│   └── logging.conf
│
├── Makefile
├── README.md
└── LICENSE
Important: configuration vs source code

The flow is:

                 boxes.conf
                     |
                     v
              Configuration
                     |
                     v
               BoxManager
                     |
          +----------+----------+
          |                     |
          v                     v
      StaticBox             DynamicBox

And:

                logging.conf
                     |
                     v
                 Logger
                     |
          +----------+----------+
          |                     |
          v                     v
       Console              Log File
                              |
                              v
                    /var/log/boxes/boxes.log

So when the application runs on the BeagleBone Black / TI AM3358, you can have:

/etc/boxes/boxes.conf
/etc/boxes/logging.conf

while the actual persistent data is:

/var/lib/boxes/boxes.data

and logs:

/var/log/boxes/boxes.log
One architectural improvement

At this stage, boxes.conf is only a configuration definition. Our current main.cpp does not yet read it.

So the next source file I recommend adding is:

app/include/Config.hpp
app/src/Config.cpp

Then the runtime flow becomes:

BeagleBone Black
       |
       v
./boxes
       |
       v
Config::load()
       |
       +------> /etc/boxes/boxes.conf
       |
       v
Configuration
       |
       v
BoxManager
       |
       +------------+
       |            |
       v            v
   StaticBox    DynamicBox
       |
       v
 Persistent Storage

That will make the project much closer to a real embedded Linux application, rather than just a C++ container demonstration.
