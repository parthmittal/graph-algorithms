#include <vector>

#include <ed-dfs.hpp>
#include <graph.hpp>

#ifndef __REDUCED_GRAPH_hpp__
#define __REDUCED_GRAPH_hpp__

namespace our {
struct fat_edge_t {
    int v, weight;
    std::vector<int> vids;
};

/* since we can't modify graphs in CRS format, but need to construct reduced
 * graph incrementally, we build it here */
struct rgraph_hack_t {
    std::vector<std::vector<fat_edge_t>> adj;
    std::vector<int> id, rid; /* id[v] <- vertex corresp. to v in reduced graph,
                                 rid[v] does the reverse */
    std::vector<int> leftV, rightV; /* left/right joint */
    std::vector<int> leftD, rightD; /* distance to left/right joint */
    std::vector<int> joint_id;      /* index of ear this node belongs to */

    rgraph_hack_t(const graph_t<int> &G, const ed_dfs::two_connected_prop &two);
    void add_vertex(int v);
};

struct reduced_graph_t : rgraph_hack_t, graph_t<fat_edge_t> {
    reduced_graph_t(const graph_t<int> &G,
                    const ed_dfs::two_connected_prop &two)
        : rgraph_hack_t(G, two), graph_t<fat_edge_t>(adj) {}
};

} // namespace our

#endif
