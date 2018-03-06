#include <iomanip>
#include <iostream>

#include <brandes.hpp>
#include <graph.hpp>

int main() {
    using namespace std;
    using namespace our;

    /* read graph from STDIN, write betweenness centrality to STDOUT */

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

    cout << fixed << setprecision(6);
    for (const double &d : brandes::bwc_all(graph)) {
        cout << d << endl;
    }
}
