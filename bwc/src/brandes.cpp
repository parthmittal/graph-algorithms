#include <queue>
#include <stack>
#include <vector>

#include <brandes.hpp>
#include <graph.hpp>

namespace our {
namespace brandes {
void bwc1(const graph_t<int> &G, int source, std::vector<double> &BC) {
    using namespace std;
    stack<int> S;
    vector<vector<int>> P(G.N);

    vector<int> sig(G.N, 0);
    sig[source] = 1;
    vector<int> d(G.N, -1);
    d[source] = 0;

    queue<int> bfq;
    bfq.push(source);
    while (!bfq.empty()) {
        int u = bfq.front();
        bfq.pop();
        S.push(u);

        for (int v : G[u]) {
            if (d[v] < 0) {
                bfq.push(v);
                d[v] = d[u] + 1;
            }
            if (d[v] == d[u] + 1) {
                // TODO: check if this overflows.
                sig[v] += sig[u];
                P[v].push_back(u);
            }
        }
    }

    vector<double> delta(G.N);
    while (!S.empty()) {
        int w = S.top();
        S.pop();
        for (auto &v : P[w]) {
            delta[v] += sig[v] / double(sig[w]) * (1 + delta[w]);
        }
        if (w != source) {
            BC[w] += delta[w];
        }
    }
}

std::vector<double> bwc_all(const graph_t<int> &G) {
    using namespace std;
    vector<double> BC(G.N);
    for (int source = 0; source < G.N; ++source) {
        bwc1(G, source, BC);
    }

    return BC;
}
} // namespace brandes
} // namespace our
