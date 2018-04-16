#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <vector>

#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>

#ifndef __BOUNDING_ECCENTRICITY_HPP__
#define __BOUNDING_ECCENTRICITY_HPP__

namespace our {
using namespace std;
/*
 * An iterative bfs implementation that returns the distance vector from the
 * source.
 * Here graph_type can take up naive graph type and CRS type.
 */
template <typename graph_type>
vector<int> ECCENTRICITY(const graph_type &G, int source, int N) {
    const int inf = 1e9;
    std::vector<int> dist(N, inf);
    std::vector<int> visited(N, false);
    dist[source] = 0;
    visited[source] = true;
    std::queue<int> q;
    // cerr << "Starting BFS from node : " << source << endl;
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

template <>
vector<int> ECCENTRICITY<reduced_LWCC_t>(const reduced_LWCC_t &G, int source,
                                          int N) {
    const int inf = 1e9;
    typedef int vertex_t;
    typedef int weight_t;
    std::vector<int> dist(N, inf);
    dist[source] = 0;
    std::set<std::pair<weight_t, vertex_t>> q;
    q.insert({dist[source], source});
    while (!q.empty()) {
        weight_t ref_w = q.begin()->first;
        vertex_t u = q.begin()->second;
        q.erase(q.begin());
        for (auto &fat_edge : G[u]) {
            vertex_t v = fat_edge.v;
            weight_t w = fat_edge.weight;
            weight_t dist_through_u = ref_w + w;
            if(dist_through_u < dist[v]) {
                q.erase({dist[v], v});
                dist[v] = dist_through_u;
                q.insert({dist[v], v});
            }
        }
    }
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
vector<int> pruning(const graph_type &G, int N, int &count) {
    std::vector<int> pruned(N, -1);
    count = 0;

    for (int i = 0; i < N; i++) {
        if (!G.in_LWCC(i))
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
                        -2; // when the neighbours of prunee are pruned
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
int select_from(int STRATEGY, const vector<int> &is_candidate,
                const graph_type &G, const vector<int> &lower,
                const vector<int> &upper, const vector<int> &d, bool &high,
                int N) {
    int to_return;
    if (STRATEGY == 1) // select the node with largest upper - lower difference,
                       // break ties with degree
    {
        int max_range = 0;
        for (int i = 0; i < N; i++) {
            if (is_candidate[i]) {
                int diff = upper[i] - lower[i];
                if (diff > max_range) {
                    to_return = i;
                    max_range = diff;
                } else if (diff == max_range &&
                           G[i].size() > G[to_return].size()) {
                    to_return = i;
                }
            }
        }
    } else if (STRATEGY == 2) // select node alternatively between max lower
    // bound and min upperbound, break ties with degree
    {
        int min_lower_bound = N, max_upper_bound = 0;
        for (int i = 0; i < N; i++) {
            if (is_candidate[i]) {
                min_lower_bound = std::min(min_lower_bound, lower[i]);
                max_upper_bound = std::max(max_upper_bound, upper[i]);
            }
        }
        // cerr << "MIN ECCENTRICITY LOWER BOUND " << min_lower_bound << endl;
        // cerr << "MAX ECCENTRICITY UPPER BOUND " << max_upper_bound << endl;
        int min_lower_node = -1, max_upper_node = -1;
        for (int i = 0; i < N; i++) {
            if (is_candidate[i]) {
                if (lower[i] == min_lower_bound &&
                    (min_lower_node == -1 ||
                     G[i].size() > G[min_lower_node].size())) {
                    // cerr << endl << "Updating min_lower_node to " << i <<
                    // endl;
                    min_lower_node = i;
                }
                // cerr << upper[i] << " ";
                if (upper[i] == max_upper_bound &&
                    (max_upper_node == -1 ||
                     G[i].size() > G[max_upper_node].size())) {
                    // cerr << "Updating max_upper_node to " << i << endl;
                    max_upper_node = i;
                }
            }
        }
        if (high) {
            high = false;
            to_return = min_lower_node;
        } else {
            high = true;
            to_return = max_upper_node;
        }
    } else if (STRATEGY == 3) // select node with the farthest distance from the
                              // current node, break ties with degree
    {
        to_return =
            std::distance(d.begin(), std::max_element(d.begin(), d.end()));
    }
    // cerr << "NEXT NODE FOR BFS " << to_return << endl;
    return to_return;
}

template <typename graph_type>
vector<int> bounding_eccentricities(const graph_type &G, int N, int STRATEGY,
                                    OptParser &parser) {
    int PRUNE = parser.checkIncluded("to_prune");
    int prune_count = 0;
    std::vector<int> pruned;
    if (PRUNE) {
        pruned = pruning(G, N, prune_count);
    }

    int candidates = G.size_LWCC() - prune_count, current = -1;
    std::vector<int> is_candidate(N, true);
    std::vector<int> eccentricity(N, 0), ecc_lower(N, 0), ecc_upper(N, N);
    std::vector<int> d;
    bool high = true;

    int number_of_iterations = 0;
    while (candidates > 0) {

        // cout << "Number of candidates left " << candidates << endl;
        if (current ==
            -1) // choose the node with max degree in the first iteration
        {
            current = 0;
            for (int i = 0; i < N; i++) {
                if (pruned[i] >= 0 || !G.in_LWCC(i)) {
                    is_candidate[i] = false;
                    ecc_lower[i] = N + 1;
                    ecc_upper[i] = -1;
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
                eccentricity[i] = ecc_lower[i];
                ecc_lower[i] = N + 1;
                ecc_upper[i] = -1;
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
    if (PRUNE) {
        for (int i = 0; i < N; i++) {
            if (!G.in_LWCC(i))
                continue;

            if (pruned[i] >= 0)
                eccentricity[i] = eccentricity[pruned[i]];
        }
    }

    std::cerr << "Number of iterations performed : " << number_of_iterations
              << endl;
    return eccentricity;
}
} // namespace our

#endif