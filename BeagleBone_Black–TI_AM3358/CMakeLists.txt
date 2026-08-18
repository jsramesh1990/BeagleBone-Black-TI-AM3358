cmake_minimum_required(VERSION 3.16)

project(
    BoxStorage
    VERSION 1.0.0
    DESCRIPTION "Date Indexed Box Storage System"
    LANGUAGES CXX
)

# ============================================================
# C++ Configuration
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warning flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# Directories
# ============================================================

set(APP_DIR       ${CMAKE_SOURCE_DIR}/app)
set(INCLUDE_DIR   ${APP_DIR}/include)
set(SRC_DIR       ${APP_DIR}/src)
set(TEST_DIR      ${APP_DIR}/tests)

# ============================================================
# Application Source
# ============================================================

set(APP_SOURCES
    ${SRC_DIR}/BoxManager.cpp
    ${SRC_DIR}/Date.cpp
    ${SRC_DIR}/DynamicBox.cpp
    ${SRC_DIR}/Entry.cpp
    ${SRC_DIR}/StaticBox.cpp
)

# ============================================================
# Application Library
# ============================================================

add_library(box_storage_lib STATIC
    ${APP_SOURCES}
)

target_include_directories(
    box_storage_lib
    PUBLIC
        ${INCLUDE_DIR}
)

# ============================================================
# Main Application
# ============================================================

add_executable(
    boxes
    ${SRC_DIR}/main.cpp
)

target_link_libraries(
    boxes
    PRIVATE
        box_storage_lib
)

# ============================================================
# Installation
# ============================================================

include(GNUInstallDirs)

install(
    TARGETS boxes
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY ${INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/box-storage
)

# ============================================================
# Configuration Files
# ============================================================

install(
    FILES
        ${CMAKE_SOURCE_DIR}/config/boxes.conf
        ${CMAKE_SOURCE_DIR}/config/logging.conf
    DESTINATION /etc/box-storage
)

# ============================================================
# Testing
# ============================================================

enable_testing()

# ------------------------------------------------------------
# Date Test
# ------------------------------------------------------------

add_executable(
    test_date
    ${TEST_DIR}/test_date.cpp
)

target_link_libraries(
    test_date
    PRIVATE
        box_storage_lib
)

add_test(
    NAME DateTest
    COMMAND test_date
)

# ------------------------------------------------------------
# StaticBox Test
# ------------------------------------------------------------

add_executable(
    test_static_box
    ${TEST_DIR}/test_static_box.cpp
)

target_link_libraries(
    test_static_box
    PRIVATE
        box_storage_lib
)

add_test(
    NAME StaticBoxTest
    COMMAND test_static_box
)

# ------------------------------------------------------------
# DynamicBox Test
# ------------------------------------------------------------

add_executable(
    test_dynamic_box
    ${TEST_DIR}/test_dynamic_box.cpp
)

target_link_libraries(
    test_dynamic_box
    PRIVATE
        box_storage_lib
)

add_test(
    NAME DynamicBoxTest
    COMMAND test_dynamic_box
)

# ------------------------------------------------------------
# Range Query Test
# ------------------------------------------------------------

add_executable(
    test_range_query
    ${TEST_DIR}/test_range_query.cpp
)

target_link_libraries(
    test_range_query
    PRIVATE
        box_storage_lib
)

add_test(
    NAME RangeQueryTest
    COMMAND test_range_query
)
