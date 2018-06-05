# for code specific to betweenness centrality
project(BetweennessCentrality)

# define SOURCES to hold all .cpp files in src
set(SOURCES src/bwc_our.cpp
            src/count_sort.cpp
            src/sssp.cpp
            src/brandes.cpp
            src/skeleton_sdm.cpp
            src/brandes_sk.cpp
            src/split_graph.cpp)

# generate static library from sources
add_library(bwc STATIC ${SOURCES})

# library bwc needs the headers in include, as does anything which uses it
target_include_directories(bwc PUBLIC include)

# library bwc uses common
target_link_libraries(bwc common)

# generate main exec for bwc
add_executable(bwc_exec main.cpp)

# add dependencies for exec; NB: this automagically makes their headers
# available to us too.
target_link_libraries(bwc_exec bwc)
