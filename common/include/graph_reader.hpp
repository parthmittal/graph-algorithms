#include <vector>
#include <optparser.hpp>
#include <graph.hpp>

#ifndef __GRAPH_READER_hpp__
#define __GRAPH_READER_hpp__

namespace our {
naive_graph_t<int> readFile(std::string filename, OptParser parser);
} // namespace our

#endif