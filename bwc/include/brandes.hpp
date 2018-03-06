#include <vector>

#include <graph.hpp>

#ifndef __BRANDES_HPP_
#define __BRANDES_HPP_

namespace our {
namespace brandes {
void bwc1(const graph_t<int> &G, int source, std::vector<double> &BC);
std::vector<double> bwc_all(const graph_t<int> &G);
} // namespace brandes
} // namespace our

#endif
