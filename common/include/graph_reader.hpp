#include <graph.hpp>
#include <vector>

#include <optparser.hpp>

#ifndef __GRAPH_READER_hpp__
#define __GRAPH_READER_hpp__

namespace our {
naive_graph_t<int> read_file(std::istream &in, OptParser &parser);
} // namespace our

#endif
