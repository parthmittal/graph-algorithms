#include <memory>
#include <queue>
#include <vector>

#include <brandes.hpp>
#include <bwc_our.hpp>
#include <count_sort.hpp>
#include <ed-dfs.hpp>
#include <optparser.hpp>
#include <sssp.hpp>

namespace our {

std::unique_ptr<rgraph_vinfo_t> bwc_our::get_node_info(int u) {
    rgraph_vinfo_t info;
    sssp(u, Gr, info.inorder, info.dist, info.num_paths);
    return std::make_unique<rgraph_vinfo_t>(info);
}

void bwc_our::sim_brandes1(int u, const rgraph_vinfo_t &Lrv,
                           const rgraph_vinfo_t &Rrv) {
    using namespace std;

    vector<int> dist(G.N, -1);
    vector<long long> num_paths(G.N, 0);

/* here, v is in V(G) */
#define SET_DISTANCE(v, distance, paths)                                       \
    do {                                                                       \
        if (dist[v] == -1 || distance < dist[v]) {                             \
            dist[v] = distance;                                                \
            num_paths[v] = 0;                                                  \
        }                                                                      \
        if (distance == dist[v]) {                                             \
            num_paths[v] += paths;                                             \
        }                                                                      \
    } while (0)

    if (G[u].size() < 2) {
        /* not biconnected, something is wrong */
        assert(0);
    } else if (Gr.id[u] != -1) {
        /* we already know the distance of every vertex in Gr
         * from u : Lrv.dist, Lrv.num_paths, etc. */
        dist[u] = 0;
        num_paths[u] = 1;

        for (int i = 0; i < G.N; ++i) {
            if (Gr.id[i] == -1) {
                /* is not in reduced graph */
                /* check path length from left and right joint of i */
                int L = Gr.id[Gr.leftV[i]], R = Gr.id[Gr.rightV[i]];
                const int dL = Gr.leftD[i] + Lrv.dist[L];
                const int dR = Gr.rightD[i] + Lrv.dist[R];
                dist[i] = min(dL, dR);

                if (dL == dist[i]) {
                    num_paths[i] += Lrv.num_paths[L];
                }
                if (dR == dist[i]) {
                    num_paths[i] += Lrv.num_paths[R];
                }
            } else {
                /* simply "import" values from Lrv */
                dist[i] = Lrv.dist[Gr.id[i]];
                num_paths[i] = Lrv.num_paths[Gr.id[i]];
            }
        }
    } else {
        // fprintf(stderr, "(%d) <(%d)-%d-(%d)> (%d)\n",
        //		Gr.leftV[u], Gr.leftD[u], u, Gr.rightD[u],
        // Gr.rightV[u]);
        const vector<int> &L = Lrv.inorder, &R = Rrv.inorder;

        num_paths[u] = 1;
        dist[u] = 0;
        if (Gr.leftV[u] != Gr.rightV[u]) {
            dist[Gr.leftV[u]] = Gr.leftD[u];
            dist[Gr.rightV[u]] = Gr.rightD[u];

            num_paths[Gr.leftV[u]] = 1;
            num_paths[Gr.rightV[u]] = 1;

            size_t i = 1, j = 1;
            while (i < L.size() && j < R.size()) {
                int v = L[i], w = R[j];
                int d1 = Gr.leftD[u] + Lrv.dist[v];
                int d2 = Gr.rightD[u] + Rrv.dist[w];
                if (d1 <= d2) {
                    SET_DISTANCE(Gr.rid[v], d1, Lrv.num_paths[v]);
                    ++i;
                } else {
                    SET_DISTANCE(Gr.rid[w], d2, Rrv.num_paths[w]);
                    ++j;
                }
            }
            while (i < L.size()) {
                int v = L[i];
                int d1 = Gr.leftD[u] + Lrv.dist[v];
                SET_DISTANCE(Gr.rid[v], d1, Lrv.num_paths[v]);
                ++i;
            }
            while (j < R.size()) {
                int w = R[j];
                int d2 = Gr.rightD[u] + Rrv.dist[w];
                SET_DISTANCE(Gr.rid[w], d2, Rrv.num_paths[w]);
                ++j;
            }
        } else if (Gr.leftV[u] == Gr.rightV[u]) {
            int v = Gr.leftV[u];
            int d1 = Gr.leftD[u], d2 = Gr.rightD[u];
            dist[v] = min(d1, d2);

            if (d1 == dist[v]) {
                num_paths[v]++;
            }
            if (d2 == dist[v]) {
                num_paths[v]++;
            }

            for (int i = 0; i < Gr.N; ++i) {
                if (i != Gr.id[v]) {
                    dist[Gr.rid[i]] = dist[v] + Lrv.dist[i];
                    num_paths[Gr.rid[i]] = num_paths[v] * Lrv.num_paths[i];
                }
            }
        }

        for (int i = 0; i < G.N; ++i) {
            if (dist[i] != -1) {
                continue;
            }
            if (Gr.id[i] == -1) {
                assert(G[i].size() == 2);

                /* first check if is in same ear-joint as u */
                if (Gr.joint_id[u] == Gr.joint_id[i]) {
                    /* two possibilities for best distance,
                     * going through vertex w/ deg >= 3 or not */

                    const int d1 = abs(Gr.leftD[u] - Gr.leftD[i]);
                    const int d2 = dist[Gr.leftV[i]] + Gr.leftD[i];
                    const int d3 = dist[Gr.rightV[i]] + Gr.rightD[i];
                    dist[i] = min(d1, min(d2, d3));

                    /* once we know the best distance, add
                     * to num_paths every way we can achieve it */

                    if (d1 == dist[i]) {
                        num_paths[i] += 1;
                    }
                    if (d2 == dist[i]) {
                        num_paths[i] += num_paths[Gr.leftV[i]];
                    }
                    if (d3 == dist[i]) {
                        num_paths[i] += num_paths[Gr.rightV[i]];
                    }
                } else {
                    /* the main idea is the same as above, except
                     * since the two vertices are on different ears,
                     * we don't have one possibility */

                    const int d1 = dist[Gr.leftV[i]] + Gr.leftD[i];
                    const int d2 = dist[Gr.rightV[i]] + Gr.rightD[i];
                    dist[i] = min(d1, d2);

                    if (d1 == dist[i]) {
                        num_paths[i] += num_paths[Gr.leftV[i]];
                    }
                    if (d2 == dist[i]) {
                        num_paths[i] += num_paths[Gr.rightV[i]];
                    }
                }
            } else {
                assert(0);
            }
        }
    }

    /* Now we know the distance for every vertex v of G:
     * - the distance of v from u
     * - the number of shortest paths from u to v
     * For the reverse phase of Brandes Algorithm, we need
     * to order the vertices in decreasing order of distance
     * from u. Since the distances are all <= |G.V|, we can
     * use counting sort.
     */

    typedef pair<int, int> pii;
    vector<pii> S(G.N);
    for (int i = 0; i < G.N; ++i) {
        S[i] = {i, dist[i]};
        assert(dist[i] < G.N);
    }
    S = counting_sort(S);
    reverse(S.begin(), S.end());

    /* We also need for each vertex v the "parents" of v,
     * which is a list of all the vertices that are adjacent
     * to v on a shortest path from u to v.
     * We don't store this explicitly, instead note that
     * u is a parent of v iff d[u] + 1 == d[v] && (u, v) is an edge of G.
     */

    vector<double> delta(G.N);
    for (auto &x : S) {
        int v = x.first;
        long long par_sum = 0;
        for (const int &w : G[v]) {
            if (dist[w] + 1 == dist[v]) {
                /* w is a parent of v */
                delta[w] += ((1 + delta[v]) * num_paths[w]) / num_paths[v];
                par_sum += num_paths[w];
            }
        }

        if (v != u) {
            bwc[v] += delta[v];
        } else {
            assert(par_sum == 0);
        }
    }

#undef SET_DISTANCE
}

void bwc_our::sim_brandes_all() {
    /* idea:
     * Do a BFS of the reduced graph (treating edges as unweighted).
     * Whenever you see edge (u, v), make sure you know rgraph_vinfo
     * and u and v (use sssp.hpp API) */

    using namespace std;

    assert(config.checkIncluded("d") != -1);

    vector<unique_ptr<rgraph_vinfo_t>> info(Gr.N);
    vector<int> vis(Gr.N);
    vector<int> done(G.N);

    int max_allocated = 0;
    int current_allocated = 0;
    for (int root = 0; root < Gr.N; ++root) {
        if (!Gr.sig[root] || vis[root]) {
            continue;
        }

        queue<int> bfq;
        bfq.push(root);
        vis[root] = 1;

        if (config.checkIncluded("d")) {
            ++current_allocated;
            max_allocated = max(max_allocated, current_allocated);
        } else {
            info[root] = get_node_info(root);
        }

        while (!bfq.empty()) {
            int u = bfq.front();
            bfq.pop();

            for (auto &e : Gr[u]) {
                int v = e.v;
                if (vis[v] == 0 && !e.vids.empty()) {
                    /* the dry run simulates the allocations we'd make */
                    bfq.push(v);
                    vis[v] = 1;

                    if (config.checkIncluded("d")) {
                        ++current_allocated;
                        max_allocated = max(max_allocated, current_allocated);
                    } else { /* not a dry run, so actually make the allocation
                              */
                        info[v] = get_node_info(v);
                    }
                }
                if (vis[v] == 1) {
                    if (!config.checkIncluded("d")) {
                        for (auto &w : e.vids) {
                            if (!done[w]) {
                                done[w] = 1;
                                sim_brandes1(w, *info[Gr.id[Gr.leftV[w]]],
                                             *info[Gr.id[Gr.rightV[w]]]);
                            }
                        }
                    }
                }
            }

            vis[u] = 2;
            if (config.checkIncluded("d")) {
                --current_allocated;
            } else {
                if (!done[Gr.rid[u]]) {
                    sim_brandes1(Gr.rid[u], *info[u], *info[u]);
                    info[u].reset(); /* calls destructor on *(info[u]) */
                }
            }
        }
    }

    for (int root = 0; root < Gr.N; ++root) {
        if (!Gr.sig[root]) {

            if (!config.checkIncluded("d")) {
                assert(!vis[root]);
                assert(!done[Gr.rid[root]]);
                /* instead of simulating for this , just run Brandes Algorithm
                 */
                brandes::bwc1(G, Gr.rid[root], bwc);
            }
        }
    }

    if (config.checkIncluded("d")) {
        fprintf(stderr, "max number of active elements was %d\n",
                max_allocated);
    }
}
}; // namespace our
