project(Testing)

set(SOURCES src/gnp.cpp)
add_library(testing STATIC ${SOURCES})
target_include_directories(testing PUBLIC include)

target_link_libraries(testing common)

add_executable(generator gen1.cpp)
add_executable(rcmp6 rcmp6.cpp)
add_executable(l2cc l2cc.cpp)

target_link_libraries(generator testing)
target_link_libraries(rcmp6 testing)
target_link_libraries(l2cc testing)

configure_file(multitest_our.sh multitest_our.sh COPYONLY)
