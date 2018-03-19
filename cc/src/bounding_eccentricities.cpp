#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>

#include <graph.hpp>
#include <optparser.hpp>

using namespace our;

/*
 * An iterative bfs implementation that returns the distance vector from the
 * source.
 * Here graph_type can take up naive graph type and CRS type.
 */
template <typename graph_type>
std::vector<int> ECCENTRICITY(const graph_type &G, int source, int N) {
    const int inf = 1e9;
    std::vector<int> dist(N, inf);
    std::vector<bool> visited(N, false);
    dist[source] = 0;
    visited[source] = true;
    std::queue<int> q;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &v : G[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    for (int i = 0; i < N; i++) {
        if (dist[i] >= inf) {
            dist[i] = -1;
        }
    }
    return dist;
}

/*
 * Function to prune the degree 1 nodes and create links to nodes that have
 * similar eccentricity
 * Returns the vector pruned where pruned[i] takes the following values:
 * -1 : if this node as well as its neighbours are not pruned
 * -2 : if the neighbours of this node are pruned
 * >= 0 : index of the node which is similar to this node
 */
template <typename graph_type>
std::vector<int> pruning(const graph_type &G, int N, int &count) {
    std::vector<int> pruned(N, -1);
    count = 0;

    for (int i = 0; i < N; i++) {
        if (!G.G.in_LWCC(i))
            continue;

        int degree = G[i].size();
        int prunee = -1;

        for (int j = 0; j < degree; j++) {
            auto it = G[i].begin();
            std::advance(it, j);
            int v = *it;
            // if degree of v is 1 and its neighbours haven't been pruned.
            if (G[v].size() == 1 && pruned[v] == -1) {
                if (prunee == -1) {
                    prunee = v; // prune all but this one
                } else {
                    pruned[v] = v;
                    count++;
                    pruned[prunee] =
                        -2; // when the neighbours of prunee are prunekkk
                }
            }
        }
    }

    std::cerr << std::endl
              << "Number of nodes pruned: " << count << std::endl
              << std::endl;
    return pruned;
}

template <typename graph_type>
int select_from(int STRATEGY, const std::vector<bool> &is_candidate,
                const graph_type &G, const std::vector<int> &lower,
                const std::vector<int> &upper, const std::vector<int> d,
                bool &high, int N) {
    int to_return;
    if (STRATEGY == 1) // select the node with largest upper - lower difference,
                       // break ties with degree
    {
        int max_range = 0;
        for (int i = 0; i < N; i++) {
            if (is_candidate) {
                int diff = upper[i] - lower[i];
                if (diff > max_range) {
                    to_return = i;
                    max_range = diff;
                } else if (diff =
                               max_range && G[i].size() > G[to_return].size()) {
                    to_return = i;
                }
            }
        }
    } else if (STRATEGY == 2) // select node alternatively between max lower
    // bound and min upperbound, break ties with degree
    {
        int max_lower_bound = *std::max_element(lower.begin(), lower.end());
        int min_upper_bound = *std::min_element(upper.begin(), upper.end());
        int max_lower_node = -1, min_upper_node = -1;
        for (int i = 0; i < N; i++) {
            if (is_candidate[i]) {
                if (lower[i] == max_lower_bound &&
                    (max_lower_node == -1 ||
                     G[i].size() > G[max_lower_node].size())) {
                    max_lower_node = i;
                }
                if (upper[i] == min_upper_bound &&
                    (min_upper_node == -1 ||
                     G[i].size() > G[min_upper_node].size())) {
                    min_upper_node = i;
                }
            }
        }
        if (high) {
            high = false;
            to_return = max_lower_node;
        } else {
            high = true;
            to_return = min_upper_node;
        }
    } else if (STRATEGY == 3) // select node with the farthest distance from the
                              // current node, break ties with degree
    {
        to_return =
            std::distance(d.begin(), std::max_element(d.begin(), d.end()));
    }
    return to_return;
}

template <typename graph_type>
std::vector<int> bounding_eccentricities(const graph_type &G, int N,
                                         int STRATEGY, OptParser &parser) {
    int PRUNE = parser.checkIncluded("to_prune");
    int prune_count = 0;
    std::vector<int> pruned;
    if (PRUNE) {
        pruned = pruning(G, N, prune_count);
    }

    int candidates = G.size_LWCC() - prune_count, current = -1;
    std::vector<bool> is_candidate(N, true);
    std::vector<int> eccentricity(N, 0), ecc_lower(N, 0), ecc_upper(N, N);
    std::vector<int> d;
    bool high = true;

    int number_of_iterations = 0;
    while (candidates > 0) {

        if (current ==
            -1) // choose the node with max degree in the first iteration
        {
            current = 0;
            for (int i = 0; i < N; i++) {
                if (pruned[i] >= 0 || !G.in_LWCC(i)) {
                    is_candidate[i] = false;
                    ecc_lower[i] = -1;
                    ecc_upper[i] = N;
                    continue;
                }
                if (G[i].size() > G[current].size()) {
                    current = i;
                }
            }
        } else {
            current = select_from(STRATEGY, is_candidate, G, ecc_lower,
                                  ecc_upper, d, high, N);
            }
        // run a bfs fron the selected node
        d = ECCENTRICITY(G, current, N);

        // calculate eccentricity of current node
        eccentricity[current] = *std::max_element(d.begin(), d.end());
        is_candidate[current] = false;
        candidates--;

        // update lower and upper bounds to connected nodes
        for (int i = 0; i < N; i++) {
            if (pruned[i] >= 0 || !G.in_LWCC(i) || d[i] == -1)
                continue;

            ecc_lower[i] = std::max(
                ecc_lower[i], std::max(eccentricity[current] - d[i], d[i]));
            ecc_upper[i] = std::min(ecc_upper[i], eccentricity[current] + d[i]);
        }

        // update candidate set
        for (int i = 0; i < N; i++) {
            if (!is_candidate[i] || !G.in_LWCC(i) || pruned[i] >= 0)
                continue;

            if (ecc_lower[i] == ecc_upper[i]) {
                is_candidate[i] = false;
                ecc_lower[i] = -1;
                ecc_upper[i] = N;
                candidates--;
            }
        }

        number_of_iterations++;

#ifdef DEBUG
        std::cerr << "---------------------------------------------------------"
                  << std::endl;
        std::cerr << "Performed BFS on " << current << std::endl;
        std::cerr << "ECCENTRICITY = " << eccentricity[current] << std::endl;
        std::cerr << "---------------------------------------------------------"
                  << std::endl;
#endif
    }
}
