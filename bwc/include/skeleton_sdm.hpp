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
    const graph_t<int> &G;
    const std::vector<int> &P; /* P[i] <- partition id of  i */
    std::vector<std::vector<wm_edge_t>> adj;
    std::vector<bool> is_frontier;
    std::vector<std::vector<int>>
        part;            /* part[i] <- vector containing all x s.t P[x] = i */
    std::vector<int> id; /* id of vertex within its partition */
    std::vector<std ::vector<int>> rid;
    std::vector<std::vector<std::vector<int>>> pDist, pNPaths;
    /* for each frontier within each partition, store the distance
     * of all other nodes within the partition.
     * possible TODO: replace w/ a dynamic matrix type? */

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
