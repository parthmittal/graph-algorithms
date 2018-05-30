#include <vector>

#include <graph.hpp>

#ifndef __SKELETON_SDM_HPP__
#define __SKELETON_SDM_HPP__

namespace our {
namespace sdm {

struct wm_edge_t {
    int v, weight;
    long long mult; /* multiplicity of edge */
};

struct sk_hack_t {
    std::vector<std::vector<wm_edge_t>> adj;
    std::vector<int> id, rid;

    const graph_t<int> &G;
    const std::vector<int> &P; /* P[i] <- partition id of  i */
    std::vector<std::vector<int>>
        part; /* part[i] <- vector containing all x s.t P[x] = i */

    /* we need these to do BFS efficiently; they represent nothing in final
     * graph */
    std::vector<int> dist;
    std::vector<long long> mult;

    sk_hack_t(const graph_t<int> &G, const std::vector<int> &P);
    void add_sp_edges(int source);
    void add_vertex(int v);
};

struct sk_graph_t : sk_hack_t, graph_t<wm_edge_t> {
    sk_graph_t(const our::graph_t<int> &G, const std::vector<int> &P)
        : sk_hack_t(G, P), graph_t<wm_edge_t>(adj) {}
};

} // namespace sdm
} // namespace our

#endif
