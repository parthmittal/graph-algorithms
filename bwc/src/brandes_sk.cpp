#include <functional>
#include <queue>
#include <stack>
#include <vector>

#include <graph.hpp>
#include <optparser.hpp>
#include <skeleton_sdm.hpp>
#include <split_graph.hpp>

namespace our {
namespace sdm {
struct queue_element_t {
    int dist, v;

    bool operator>(const queue_element_t &other) const {
        return (dist > other.dist);
    }
};

void brandes_skone(const split_graph_t &G, int source,
                   const std::vector<bool> &is_dest, std::vector<double> &bc) {
    using namespace std;
    vector<int> dist(G.N, -1), num_paths(G.N);
    vector<bool> popped(G.N);

    typedef queue_element_t qet;
    priority_queue<qet, vector<qet>, greater<qet>> dfq;

    dist[source] = 0;
    num_paths[source] = 1;
    dfq.push({0, source});

    stack<int> S;

    while (!dfq.empty()) {
        int u = dfq.top().v;
        dfq.pop();

        if (!popped[u]) {
            popped[u] = 1;
            S.push(u);

            for (int i = 0; i < G.adj_size(u); ++i) {
                auto e = G.get_edge(u, i);
                int v = e.v;
                int weight = dist[u] + e.weight;
                if (dist[v] == -1 || weight < dist[v]) {
                    dist[v] = weight;
                    num_paths[v] = 0;
                    dfq.push({weight, v});
                }
                if (weight == dist[v]) {
                    num_paths[v] += num_paths[u] * e.mult;
                }
            }
        }
    }

    vector<double> delta(G.N);

    while (!S.empty()) {
        int v = S.top();
        S.pop();
        for (int i = 0; i < G.adj_size(v); ++i) {
            auto e = G.get_edge(v, i);
            int w = e.v;
            if (dist[w] + e.weight == dist[v]) {
                delta[w] += e.mult * num_paths[w] / double(num_paths[v]) *
                            (is_dest[v] + delta[v]);
            }
        }
    }

    const auto &sk = G.skeleton;
    for (int p = 0; p < sk.part.size(); ++p) {
        if (!G.active[p]) {
            /* find frontier barrier */
            int F = 0;
            for (; F < sk.part[p].size() && sk.is_frontier[sk.rid[p][F]]; ++F)
                ;

            for (int v = F; v < sk.part[p].size(); ++v) {
                int va = sk.rid[p][v];
                int min_dist = 1e9, sigma = 0;
                for (int f = 0; f < F; ++f) {
                    int fa = sk.rid[p][f];
                    if (dist[fa] + sk.pDist[p][f][v] < min_dist) {
                        min_dist = dist[fa] + sk.pDist[p][f][v];
                        sigma = 0;
                    }
                    if (dist[fa] + sk.pDist[p][f][v] == min_dist) {
                        sigma += num_paths[fa] * sk.pNPaths[p][f][v];
                    }
                }
                for (int f = 0; f < F; ++f) {
                    int fa = sk.rid[p][f];
                    if (dist[fa] == sk.pDist[p][f][v] + min_dist) {
                        double old = delta[va];
                        delta[va] += sigma / double(num_paths[fa]) *
                                     sk.pNPaths[p][f][v] * delta[fa];

                        if (va == 19 && old != delta[va]) {
                            int a = 5;
                        }
                    }
                }
            }
        }
    }

    for (int v = 0; v < G.N; ++v) {
        if (v != source) {
            if (v == 19 && delta[v] != 0) {
                int a = 5;
            }
            bc[v] += delta[v];
        }
    }
}

std::vector<double> brandes_skall(const sk_graph_t &G) {
    using namespace std;
    stack<int> S;

    split_graph_t split(G);

    vector<bool> is_dest(G.N);
    vector<double> bc(G.N);
    for (int i = 0; i < G.part.size(); ++i) {
        for (int j = 0; j < G.part.size(); ++j) {
            split.active[i] = 1;
            split.active[j] = 1;

            for (int v : G.part[j]) {
                is_dest[v] = true;
            }

            /******** SETUP COMPLETE ***********/

            for (int si = 0; si < G.part[i].size(); ++si) {
                int s = G.rid[i][si];
                brandes_skone(split, s, is_dest, bc);
            }

            /******** CLEANUP BEGINS ***********/
            if (config.checkIncluded("verbose")) {
                cerr << i << ' ' << j << " done" << endl;
            }

            for (int v : G.part[j]) {
                is_dest[v] = false;
            }

            split.active[i] = 0;
            split.active[i] = 0;
        }

        if (config.checkIncluded("verbose")) {
            cerr << i << " done" << endl;
        }
    }

    return bc;
}
} // namespace sdm
} // namespace our
