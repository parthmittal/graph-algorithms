#include <vector>

#include <graph.hpp>
#include <skeleton_sdm.hpp>

#ifndef __SPLIT_GRAPH_HPP__
#define __SPLIT_GRAPH_HPP__

namespace our {
namespace sdm {
struct split_graph_t {
    const graph_t<int> &G;
    const sk_graph_t &skeleton;
    std::vector<int> active; /* active[i] = 1 iff i-th partition is "active" */

    split_graph_t(const sk_graph_t &skeleton)
        : G(skeleton.G), skeleton(skeleton), active(skeleton.part.size()) {}

    int adj_size(int u);
    wm_edge_t get_edge(int u, int i); /* get i-th edge incident on u */
};
} // namespace sdm
} // namespace our

#endif
