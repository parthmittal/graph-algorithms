#include <memory>
#include <vector>

#include <ed-dfs.hpp>
#include <graph.hpp>
#include <reduced_graph.hpp>

#ifndef __BWC_OUR_hpp__
#define __BWC_OUR_hpp__

namespace our {

struct rgraph_vinfo_t {
    std::vector<int> dist; /* dist[i] <- distance of i from this vertex in Gr */
    std::vector<long long> num_paths; /* number of shortest paths */
    std::vector<int> inorder; /* vertices of Gr ordered by distance from this */
};

/* same as rgraph_vinfo_t, except for full graph */
struct graph_vinfo_t {
    std::vector<int> dist;
    std::vector<long long> num_paths;
    std::vector<int> inorder;
};

struct bwc_our {
    const graph_t<int> &G;
    ed_dfs::two_connected_prop two;
    reduced_graph_t Gr;

    std::vector<double> bwc;

    void forward_phase_reduced_graph();
    void sim_forward_phase(int u);
    void sim_brandes1(int u, const rgraph_vinfo_t &Lrv,
                      const rgraph_vinfo_t &Rrv);
    std::unique_ptr<graph_vinfo_t> get_node_info(int u);
    std::unique_ptr<rgraph_vinfo_t> get_rnode_info(int u);

    void sim_brandes_ej(int L, int R, const graph_vinfo_t &LI,
                        const graph_vinfo_t &RI, const std::vector<int> &ear);

    void sim_brandes_all();

    bwc_our(graph_t<int> &G) : G(G), two(G), Gr(G, two), bwc(G.N) {}
};
}; // namespace our

#endif
