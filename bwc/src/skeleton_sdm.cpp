#include <queue>
#include <vector>

#include <graph.hpp>
#include <skeleton_sdm.hpp>

using namespace std;

namespace our {
namespace sdm {
sk_hack_t::sk_hack_t(const graph_t<int> &G, const vector<int> &P)
    : P(P), G(G), id(G.N, -1) {
    for (int u = 0; u < G.N; ++u) {
        for (const int &v : G[u]) {
            if (P[u] != P[v]) { /* belong to different partitions */
                add_vertex(u);
                add_vertex(v);
                adj[id[u]].push_back({id[v], 1, 1});
            }
        }
    }

    for (int u = 0; u < G.N; ++u) {
        part[P[u]].push_back(u);
    }

    for (int u = 0; u < G.N; ++u) {
        if (id[u] != -1) {
            add_sp_edges(u);
        }
    }
}

void sk_hack_t::add_vertex(int v) {
    if (id[v] == -1) {
        rid.push_back(v);
        id[v] = adj.size();
        adj.push_back({});
    }
}

void sk_hack_t::add_sp_edges(int source) {
    typedef long long ll;

    int pid = P[source];

    for (auto &v : part[pid]) {
        dist[v] = G.N + 1;
        mult[v] = 0;
    }

    queue<int> bfq;
    bfq.push(source);
    dist[source] = 0;
    mult[source] = 1;

    while (!bfq.empty()) {
        int u = bfq.front();
        bfq.pop();
        for (const int &v : G[u]) {
            if (P[v] != pid) {
                continue;
            }
            if (dist[v] > dist[u] + 1) {
                dist[v] = dist[u] + 1;
                mult[v] = 0;
                if (id[v] == -1) {
                    bfq.push(v);
                }
            }
            if (dist[v] == dist[u] + 1) {
                mult[v] += mult[u];
            }
        }
    }

    for (const int &v : part[source]) {
        if (dist[v] < G.N + 1) {
            adj[source].push_back({v, dist[v], mult[v]});
        }
    }
}
} // namespace sdm
} // namespace our
