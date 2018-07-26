#include <queue>
#include <vector>

#include <graph.hpp>
#include <skeleton_sdm.hpp>

using namespace std;

namespace our {
namespace sdm {
sk_hack_t::sk_hack_t(const graph_t<int> &G, const vector<int> &P)
    : G(G), P(P), adj(G.N), is_frontier(G.N), id(G.N), dist(G.N), mult(G.N) {
    int mxP = 0;
    for (int u = 0; u < G.N; ++u) {
        mxP = max(mxP, P[u]);
    }
    part.resize(mxP + 1);
    pDist.resize(mxP + 1);
    pNPaths.resize(mxP + 1);
    rid.resize(mxP + 1);

    for (int u = 0; u < G.N; ++u) {
        for (const int &v : G[u]) {
            if (P[u] != P[v]) { /* belong to different partitions */
                is_frontier[u] = true;
                is_frontier[v] = true;
                adj[u].push_back({v, 1, 1});
            }
        }
    }

    for (int u = 0; u < G.N; ++u) {
        part[P[u]].push_back(u);
    }

    /* compute id of each vertex within its partition; we ensure that
     * withing partition P_i, the frontiers F_i, get ids 0..|F_i| - 1,
     * as this makes it easier to look up distances from frontier nodes to
     * other nodes of the partition */
    vector<int> cnt(part.size());
    for (int u = 0; u < G.N; ++u) {
        if (is_frontier[u]) {
            int p = P[u];
            id[u] = cnt[p]++;
            rid[p].push_back(u);
            pDist[p].push_back(vector<int>(part[p].size()));
            pNPaths[p].push_back(vector<int>(part[p].size()));
        }
    }

    for (int u = 0; u < G.N; ++u) {
        if (!is_frontier[u]) {
            int p = P[u];
            id[u] = cnt[p]++;
            rid[p].push_back(u);
        }
    }

    for (int u = 0; u < G.N; ++u) {
        if (is_frontier[u]) {
            add_sp_edges(u);
        }
    }
}

void sk_hack_t::add_sp_edges(int source) {
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
                if (!is_frontier[v]) {
                    bfq.push(v);
                }
            }
            if (dist[v] == dist[u] + 1) {
                mult[v] += mult[u];
            }
        }
    }

    for (const int &v : part[pid]) {
        if (v == source) {
            continue;
        }
        if (dist[v] < G.N + 1 && is_frontier[v]) {
            adj[source].push_back({v, dist[v], mult[v]});
        } else {
            pDist[pid][id[source]][id[v]] = dist[v];
            pNPaths[pid][id[source]][id[v]] = mult[v];
        }
    }
}
} // namespace sdm
} // namespace our
