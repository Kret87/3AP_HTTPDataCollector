cmake_minimum_required(VERSION 3.16)

project(3AP_HTTPDataCollector)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)

add_executable(
    3AP_HTTPDataCollector
    src/main.cpp
    src/HttpClient.cpp
    src/Database.cpp
    src/Configuration.cpp
)

target_include_directories(
    3AP_HTTPDataCollector
    PRIVATE
    ${PQXX_INCLUDE_DIRS}
)

target_link_directories(
    3AP_HTTPDataCollector
    PRIVATE
    ${PQXX_LIBRARY_DIRS}
)

target_link_libraries(
    3AP_HTTPDataCollector
    PRIVATE
    CURL::libcurl
    ${PQXX_LIBRARIES}
)
