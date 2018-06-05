#include <cstdlib>

#include <graph.hpp>
#include <skeleton_sdm.hpp>
#include <split_graph.hpp>

namespace our {
namespace sdm {
int split_graph_t::adj_size(int u) const {
    int pid = skeleton.P[u];

    if (active[pid]) { /* partition containing u is "active" */
        return G[u].size();
    } else if (skeleton.is_frontier[u]) {
        return skeleton[u].size();
    } else {
        return 0;
    }
}

wm_edge_t split_graph_t::get_edge(int u, int i) const {
    int pid = skeleton.P[u];

    if (active[pid]) {
        return {G[u][i], 1, 1};
    } else if (skeleton.is_frontier[u]) {
        return skeleton[u][i];
    } else {
        exit(-1);
    }
}
} // namespace sdm
} // namespace our
