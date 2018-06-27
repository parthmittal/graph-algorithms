#include <vector>

#include <graph.hpp>
#include <skeleton_sdm.hpp>

#ifndef __BRANDES_SK_hpp__
#define __BRANDES_SK_hpp__

namespace our {
namespace sdm {
std::vector<double> brandes_skall(const sk_graph_t &G);
}
} // namespace our

#endif
