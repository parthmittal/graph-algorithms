# for code specific to closenness centrality
project(ClosenessCentrality)

# define SOURCES to hold all .cpp files in src
set(SOURCES )

# generate static library from sources
add_library(cc STATIC ${SOURCES})

# library cc needs the headers in include, as does anything which uses it
target_include_directories(cc PUBLIC include)

# library cc uses common
target_link_libraries(cc common)

# generate main exec for cc
add_executable(cc_exec main.cpp)

# add dependencies for exec; NB: this automagically makes their headers
# available to us too.
target_link_libraries(cc_exec cc)
