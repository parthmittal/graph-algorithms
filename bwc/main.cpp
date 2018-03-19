#include <iomanip>
#include <iostream>

#include <brandes.hpp>
#include <ed-dfs.hpp>
#include <graph.hpp>

int main() {
    using namespace std;
    using namespace our;

    /* read graph from STDIN, write ear decomposition to STDOUT */

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
    ed_wrapper.ear_decompose();

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
}
