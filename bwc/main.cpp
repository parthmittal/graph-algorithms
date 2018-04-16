#include <iomanip>
#include <iostream>

#include <brandes.hpp>
#include <bwc_our.hpp>
#include <ed-dfs.hpp>
#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>
#include <reduced_graph.hpp>

namespace our {
    OptParser config;
};

int main(int argc, const char **argv) {
    using namespace std;
    using namespace our;

    config.addOption("-d", "--dry-run");
    config.parse(argc, argv);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> adj(N);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    graph_t<int> graph(adj);

    ed_dfs::two_connected_prop ed_wrapper(graph);

    /* iterate over biconnected components */
    int cid = 1; /* component id */
    for (auto &bcc : ed_wrapper.ear_decomposition) {
        cout << "BCC #" << cid << endl;
        int eid = 1; /* ear id within component */
        for (auto &ear : bcc) {
            cout << "    Ear #" << eid << '\n';
            for (auto &edge : ear) {
                cout << "        " << edge.first << " -> " << edge.second
                     << '\n';
            }
            ++eid;
        }
        ++cid;
    }

    reduced_graph_t rgraph(graph, ed_wrapper);
    for (int u = 0; u < rgraph.N; ++u) {
        for (auto &e : rgraph[u]) {
            printf("%d (%d)", rgraph.rid[u], u);
            for (auto &w : e.vids) {
                printf(" -- %d", w);
            }
            printf(" -- %d (%d)\n", rgraph.rid[e.v], e.v);
        }
    }

    bwc_our bc_wrapper(graph);
    bc_wrapper.sim_brandes_all();

    cout << fixed << setprecision(5);
    for (int i = 0; i < graph.N; ++i) {
        cout << bc_wrapper.bwc[i] << '\n';
    }
}
