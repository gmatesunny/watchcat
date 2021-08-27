cmake_minimum_required (VERSION 2.8.11)
project(watchcat)
set(CMAKE_BUILD_TYPE Release)

enable_testing()

add_subdirectory(example)
add_subdirectory(test)
add_subdirectory(libwatchcat)