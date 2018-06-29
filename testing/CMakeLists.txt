project(Testing)

set(SOURCES src/gnp.cpp)
add_library(testing STATIC ${SOURCES})
target_include_directories(testing PUBLIC include)

target_link_libraries(testing common)

add_executable(generator gen1.cpp)
add_executable(rcmp6 rcmp6.cpp)

target_link_libraries(generator testing)
target_link_libraries(rcmp6 testing)

configure_file(multitest_our.sh multitest_our.sh COPYONLY)
