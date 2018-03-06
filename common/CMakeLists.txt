project(CommonStuff)

# define SOURCES to hold all .cpp files in src
file(GLOB SOURCES "src/*.cpp")

# generate static library from sources
add_library(common STATIC ${SOURCES})

# library common needs the headers in include, as does anything which uses it
target_include_directories(common PUBLIC include)
