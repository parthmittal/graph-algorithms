cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -pedantic -Werror")
project(CentralityThings)

add_subdirectory(bwc)
add_subdirectory(common)
add_subdirectory(testing)
# add_subdirectory(cc)
