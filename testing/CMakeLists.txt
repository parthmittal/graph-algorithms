project(Testing)

set(SOURCES src/gnp.cpp)
add_library(testing STATIC ${SOURCES})
target_include_directories(testing PUBLIC include)

target_link_libraries(testing common)

add_executable(generator gen1.cpp)

target_link_libraries(generator testing)

configure_file(multitest_our.sh multitest_our.sh COPYONLY)
