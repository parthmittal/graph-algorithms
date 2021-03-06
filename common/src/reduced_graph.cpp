#include <cassert>
#include <vector>

#include <graph.hpp>
#include <optparser.hpp>
#include <reduced_graph.hpp>

namespace our {

rgraph_hack_t::rgraph_hack_t(const graph_t<int> &G,
                             const ed_dfs::two_connected_prop &two)
    : id(G.N, -1), leftV(G.N, -1), rightV(G.N, -1), leftD(G.N), rightD(G.N),
      joint_id(G.N), sig() {
    using namespace std;
    using namespace ed_dfs;

    /* NB: currently we expect that G is two-connected */
    if (two.ear_decomposition.size() > 1 && config.checkIncluded("verbose")) {
        cerr << "WARNING: Graph isn't biconnected, working with first "
                "biconnected component instead"
             << endl;
    }

    auto &ED = two.ear_decomposition[0];
    int eid = 1;
    for (const path_t &ear : ED) {
        assert(ear.size() >= 1);

        /* increase the degree of the beginning and end of ear,
         * so even if their degree was only 2, they still become end-points
         * of edges in Gr */
        int ear_start = ear.front().first;

        int weight = 1, u = ear_start;
        vector<int> active;
        for (auto &e : ear) {
            if (weight > 0 && (G[e.second].size() >= 3 || e == ear.back())) {
                /* add an edge between u and e.v */
                int v = e.second;

                /* NB: this does nothing if the vertices are already
                 * in the reduced graph */
                add_vertex(u);
                add_vertex(v);

                adj[id[u]].push_back({id[v], weight, active});

                if (u != v) { /* if edge corresp. to cycle, only add once */
                    reverse(active.begin(), active.end());
                    adj[id[v]].push_back({id[u], weight, active});
                    reverse(active.begin(), active.end());
                }

                if (weight >= 2) {
                    sig[id[u]] = 1;
                    sig[id[v]] = 1;
                }

                int temp = 1;
                for (auto &w : active) {
                    /* these are all the vertices between u and v
                     * on this ear */
                    leftV[w] = u;
                    rightV[w] = v;
                    leftD[w] = temp;
                    rightD[w] = weight - temp;
                    joint_id[w] = eid;
                    // fprintf(stderr, "%d %d %d\n", w, distL[w], distR[w]);
                    ++temp;
                }
                active.clear();
                ++eid;

                u = v;
                weight = 1;
            } else {
                active.push_back(e.second);
                ++weight;
            }
        }
        /* make sure we did collapse all the edges */
        assert(u == ear.back().second);
    }
    if (config.checkIncluded("verbose")) {
        fprintf(stderr, "constructed reduced graph, with %lu vertices\n",
                adj.size());
    }
}

void rgraph_hack_t::add_vertex(int v) {
    if (id[v] == -1) {
        rid.push_back(v);
        id[v] = adj.size();
        adj.push_back({});
        sig.push_back(0);
    }
}
} // namespace our
