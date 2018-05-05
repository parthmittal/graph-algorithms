#include <map>
#include <stack>
#include <vector>
#include <set>

#include <ed-dfs.hpp>
#include <graph.hpp>
#include <compress.hpp>

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
    std::vector<int> sig; /* does vertex v have at least one edge w/ weight > 1
                             incident on it */

    rgraph_hack_t(const graph_t<int> &G, const ed_dfs::two_connected_prop &two);
    void add_vertex(int v);
};

struct reduced_graph_t : rgraph_hack_t, graph_t<fat_edge_t> {
    std::map<int, int> vert;
    bool in_LWCC(int i) const { return vert.find(i) != vert.end(); }
    int size_LWCC() const { return vert.size(); }

    reduced_graph_t(const graph_t<int> &G,
                    const ed_dfs::two_connected_prop &two)
        : rgraph_hack_t(G, two), graph_t<fat_edge_t>(adj) {}

    void set_vert(const std::map<int, int> &ver) { vert = ver; }
};

struct BCC_t {
    std::vector<std::vector<int>> adj;
    const static int MAX_N = 5e6 + 10;
    std::map<int, int> vert;

    BCC_t(const naive_graph_t<int> &G) {
        vis.resize(G.N);
        ent.resize(G.N);
        low.resize(G.N);
        p.resize(G.N);
        int timer = 1;
        for (int i = 0; i < G.N; i++) {
            if (!vis[i])
                dfs(G, i, timer);
        }

        for (auto &B : bcc) {
            if (B.size() > all_edges.size()) {
                all_edges = B;
            }
        }

        std::set<int> temp;
        for(auto &e : all_edges) {
            temp.insert(e.first);
            temp.insert(e.second);
        }

        int size_bcc = 0;
        std::map<int, int> key = compress_list(temp, size_bcc);
        adj.resize(size_bcc);

        for (auto &e : all_edges) {
            vert[key[e.first]];
            vert[key[e.second]];
            adj[key[e.first]].push_back(key[e.second]);
            adj[key[e.second]].push_back(key[e.first]);
        }
    }

  private:
    typedef std::pair<int, int> pii;
    std::stack<pii> S;
    std::vector<std::vector<pii>> bcc;
    std::vector<pii> all_edges;
    std::vector<int> vis, ent, low, p;

    void dfs(const naive_graph_t<int> &graph, int node, int &timer);
    void store_bcc(int u, int v);
};

} // namespace our

#endif
