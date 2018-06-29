project(Testing)

set(SOURCES src/gnp.cpp)
add_library(testing STATIC ${SOURCES})
target_include_directories(testing PUBLIC include)

target_link_libraries(testing common)

add_executable(generator gen1.cpp)
add_executable(rcmp6 rcmp6.cpp)
add_executable(l2cc l2cc.cpp)
add_executable(pe2a port_edgelist_adj.cpp)
add_executable(ppart port_partition.cpp)

target_link_libraries(generator testing)
target_link_libraries(rcmp6 testing)
target_link_libraries(l2cc testing)
target_link_libraries(pe2a testing)

configure_file(multitest_our.sh multitest_our.sh COPYONLY)
configure_file(multitest_erdos.sh multitest_erdos.sh COPYONLY)
