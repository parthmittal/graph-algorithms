#include <cstdlib>

#include <graph.hpp>
#include <skeleton_sdm.hpp>
#include <split_graph.hpp>

namespace our {
namespace sdm {
int split_graph_t::adj_size(int u) {
    int pid = skeleton.P[u];
    int fid = skeleton.id[u];

    if (active[pid]) { /* partition containing u is "active" */
        return G[u].size();
    } else if (fid != -1) { /* u is a frontier */
        return skeleton[fid].size();
    } else {
        return 0;
    }
}

wm_edge_t split_graph_t::get_edge(int u, int i) {
    int pid = skeleton.P[u];
    int fid = skeleton.id[u];

    if (active[pid]) {
        assert(i < G[u].size());
        return {G[u][i], 1, 1};
    } else if (fid != -1) {
        return skeleton[fid][i];
    } else {
        exit(-1);
    }
}
} // namespace sdm
} // namespace our
