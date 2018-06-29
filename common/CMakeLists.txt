project(CommonStuff)

set(SOURCES src/ed-dfs.cpp
            src/graph.cpp
            src/reduced_graph.cpp
            src/compress.cpp
            src/config.cpp
            src/graph_reader.cpp
            src/optparser.cpp)

# generate static library from sources
add_library(common STATIC ${SOURCES})

# library common needs the headers in include, as does anything which uses it
target_include_directories(common PUBLIC include)
