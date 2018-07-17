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

std::unique_ptr<rgraph_vinfo_t> bwc_our::get_rnode_info(int u) {
    rgraph_vinfo_t info;
    sssp(u, Gr, info.inorder, info.dist, info.num_paths);
    return std::make_unique<rgraph_vinfo_t>(info);
}

std::unique_ptr<graph_vinfo_t> bwc_our::get_node_info(int u) {
    graph_vinfo_t info;
    bfs(u, G, info.inorder, info.dist, info.num_paths);
    return std::make_unique<graph_vinfo_t>(info);
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

/* L and R are the two end-points of the ear joint
 * NB: L, R are both in Gr, not G
 *
 * Plan:
 * 1. L <- vertex corresp. to L in G, same for R
 * 2. No need to do BFS, just retrieve info from graph_vinfo_t.
 * 2. In the reverse phase, do some magic to also simulate Brandes
 * for all the vertices in ear.
 */
void bwc_our::sim_brandes_ej(int L, int R, const graph_vinfo_t &LI,
                             const graph_vinfo_t &RI,
                             const std::vector<int> &ear) {
    using namespace std;

    L = Gr.id[L];
    R = Gr.id[R];

    int sources[] = {L, R};

    int eid = two.ears_of[ear.front()];

    /* case1: for each u not in current ear, compute
     * sum_{s in current ear} sum_{t not in current ear, t != u} bwc at u due
     * to pair s, t
     */
    for (int i = 0; i < 2; ++i) {
        auto root = (i == 0) ? LI : RI;
        auto other = (i == 0) ? RI : LI;
        vector<double> delta(G.N, 0);
        vector<double> delta_eq(G.N, 0);

        /* iterate on bfs-tree of root in non-increasing order of distance */
        for (int v : root.inorder) {
            if (two.ears_of[v] == eid) {
                continue;
                /* this node is part of the ear we're using as a source;
                 * we deal with such vertices in a separate case
                 */
            }

            int root_distance = root.dist[v];
            int other_distance = other.dist[v];
            int ear_size = ear.size();

            /* we want to find largest x s.t.
             * root_distance + x < (ear_size - x + 1) + other_distance
             * => 2x < ear_size + 1 + other_distance - root_distance
             * => x < (ear_size + 1 + other_distance - root_distance) / 2
             * and, of course, x <= ear_size
             */

            int x2 = ear_size + 1 + other_distance - root_distance;
            int x = x2 / 2 - (x2 % 2 == 0); /* x < x2 / 2 */
            x = min(x, ear_size);
            x = max(x, 0);

            /* now test if (x + 1) leads to equality */

            int eqn = 0;
            if (x < ear_size) {
                eqn =
                    (x + 1 + root_distance == other_distance + (ear_size - x));
                /* when you move to (x + 1), the distance from the other side
                 * is other_distance + (ear_size - (x + 1) + 1) */
            }

            for (auto &u : G[v]) {
                if (root.dist[u] + 1 == root.dist[v]) {
                    delta[u] += root.num_paths[u] / double(root.num_paths[v]) *
                                (x + delta[v]);
                    /* we add (x + delta[v]) instead of (1 + delta[v]),
                     * since we have x sources which contribute bwc with v
                     * as sink
                     */

                    if (eqn != 0) {
                        /* deal with situation with equal length path via root
                         * and via other:
                         * root.num_paths[u] paths go via root and u into v
                         * root.num_paths[v] + other.num_paths[v] go from the
                         * equal length source from either side. Note that
                         * whenever eqn is non-zero, these paths are distinct.
                         */

                        /* let s = (x + 1)-th node on the ear from root towards
                         * other;
                         * we know that there exist shortest paths
                         * s --- root --- v and s --- other --- v;
                         * we want to know if there exist shortest paths
                         * s --- root --- u and s --- other --- u.
                         *
                         * The situation looks something like:
                         * root ----- s ----- other
                         *    \               /
                         *     \             /
                         *      \           /
                         *       \         /
                         *        \       /
                         *         \     /
                         *         common
                         *           |
                         *           |
                         *           |
                         *           u
                         *           |
                         *           v
                         * This changes the "propagation" of the betweenness
                         * centrality.
                         */
                        if (root.dist[u] + x ==
                            other.dist[u] + (ear_size - x)) {
                            delta_eq[u] +=
                                root.num_paths[u] /
                                double(root.num_paths[v] + other.num_paths[v]) *
                                (1 + delta_eq[v]);
                        } else {
                            delta[u] +=
                                root.num_paths[u] /
                                double(root.num_paths[v] + other.num_paths[v]) *
                                (1 + delta_eq[v]);
                        }
                    }
                }
            }

            bwc[v] += delta[v] + delta_eq[v];
        }
    }

    /* case2: Same as case1, but for each u __in__ current ear */
    for (int i = 0; i < 2; ++i) {
        auto root = (i == 0) ? LI : RI;
        auto other = (i == 0) ? RI : LI;

        int ear_size = ear.size();
        vector<double> offset(ear_size + 1), update(ear_size + 1);

        if (ear_size < 2) {
            /* if the ear has only 1 vertex, then it does not contribute any
             * betweenness centrality to itself */
            continue;
        }

        for (int t = 0; t < G.N; ++t) {
            if (two.ears_of[t] == eid) { /* t belongs to same ear */
                continue;
            }

            /* as before, compute the "cut-point" x */
            int root_distance = root.dist[t];
            int other_distance = other.dist[t];

            int x2 = ear_size + 1 + other_distance - root_distance;
            int x = x2 / 2 - (x2 % 2 == 0); /* x < x2 / 2 */
            x = min(x, ear_size);

            /* with the first x vertices of the ear from root as source,
             * each vertex before them gets a betweenness centrality of 1
             * due to the sink t
             */

            if (x <= 1) {
                continue;
            }

            offset[0] += x - 1; /* (x - 1) sources, one sink, all add +1 */
            update[1] -= 1;     /* the number of sources decrease by 1 */
            update[x + 1] += 1; /* once cross last of sources, stop decreasing
                                   number of sources */
        }

        vector<double> delta(ear_size);
        double diff = 0, val = 0;
        for (int d = 0; d < ear_size; ++d) {
            diff += update[d];
            val += offset[d];
            val += diff;

            delta[d] += val;
        }

        if (i == 1) {
            reverse(delta.begin(), delta.end());
        }

        for (int i = 0; i < ear_size; ++i) {
            bwc[ear[i]] += delta[i];
        }
    }
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
            info[root] = get_rnode_info(root);
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
                        info[v] = get_rnode_info(v);
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
                if (!config.checkIncluded("s")) {
                    /* instead of simulating for this , just run Brandes
                     * Algorithm
                     */
                    brandes::bwc1(G, Gr.rid[root], bwc);
                } else {
                    info[root] = get_rnode_info(root);
                    sim_brandes1(Gr.rid[root], *info[root], *info[root]);
                    info[root].reset();
                }
            }
        }
    }

    if (config.checkIncluded("d")) {
        fprintf(stderr, "max number of active elements was %d\n",
                max_allocated);
    }
}
}; // namespace our
