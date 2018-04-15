#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

#ifndef __RV_hpp__
#define __RV_hpp__

namespace our {
template <typename graph_type>
void bfs(int source, int root, const graph_type &G,
         std::vector<std::vector<int>> &dist,
         std::vector<std::vector<int>> &visited, std::vector<int> &eccentricity,
         bool update_eccentricity) {
    dist[root][source] = 0;
    if (update_eccentricity)
        eccentricity[source] =
            std::max(eccentricity[source], dist[root][source]);
    visited[root][source] = true;
    std::queue<int> q;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &i : G[u]) {
            if (!visited[root][i]) {
                dist[root][i] = dist[root][u] + 1;
                if (update_eccentricity)
                    eccentricity[i] = std::max(eccentricity[i], dist[root][i]);
                q.push(i);
            }
        }
    }
}

template <typename graph_type>
std::vector<int> modified_bfs(int source, const graph_type &G,
                              std::vector<int> &dist, std::vector<int> &visited,
                              std::vector<int> &eccentricity,
                              std::set<int> &Ns_w, int s, int N) {
    std::vector<int> parent;
    std::vector<int> closest_on_path(N);
    dist[source] = 0;
    eccentricity[source] = std::max(eccentricity[source], dist[source]);
    visited[source] = true;
    parent[source] = -1;
    std::queue<int> q;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &i : G[u]) {
            if (!visited[i]) {
                dist[i] = dist[u] + 1;
                eccentricity[i] = std::max(eccentricity[i], dist[i]);
                parent[i] = u;
                if (Ns_w.size() < s)
                    Ns_w.insert(i);
                else {
                    int p = parent[i];
                    while (Ns_w.find(p) == Ns_w.end()) {
                        p = parent[p];
                    }
                    closest_on_path[i] = p;
                }
                visited[i] = true;
                q.push(i);
            }
        }
    }
    return closest_on_path;
}

/*
 * RV algorithm based on the paper by roddity and vassilevska
 * gives an approximation to eccentricities of the vertices of the graph
 * Running time: O(m * sqrt(n log n))
 * Memorty: \theta (m * sqrt(n log n))
 */
template <typename graph_type>
std::vector<int> RV_algorithm(const graph_type &G, int N, int s) {

    /* sample s vertices from the largest WCC */
    std::vector<int> sampled_vertices;
    std::set<int> temp_sample;
    std::vector<int> found(N, false);
    srand((int)time(0));
    while (temp_sample.size() < s) {
        int a = (rand() % N);
        if (G.in_LWCC(a)) {
            temp_sample.insert(a);
        }
    }
    std::cerr << s << " vertices sampled" << endl;
    for (auto &i : temp_sample) {
        sampled_vertices.push_back(i);
        found[i] = true;
    }

    /* perform bfs from the sampled vertices */
    const int inf = -1;
    std::vector<std::vector<int>> dist(s + 1, std::vector<int>(N, inf));
    std::vector<std::vector<int>> visited(s + 1, std::vector<int>(N, false));
    std::vector<int> eccentricity(N, 0);
    for (int i = 0; i < s; i++) {
        bfs(sampled_vertices[i], i, G, dist, visited, eccentricity, true);
    }

    int max_distance = 0;
    int w, ps_w;
    for (int u = 0; u < N; u++) {
        if (G.in_LWCC(u)) {
            for (int i = 0; i < s; i++) {
                if (dist[i][u] > max_distance) {
                    max_distance = dist[i][u];
                    w = u;
                    ps_w = i;
                }
            }
        }
    }
    found[w] = true;
    int min_ecc = INT_MAX;
    for (int i = 0; i < s; i++) {
        eccentricity[sampled_vertices[i]] =
            *std::max_element(dist[i].begin(), dist[i].end());
        min_ecc = std::min(min_ecc, eccentricity[sampled_vertices[i]]);
    }

    std::set<int> Ns_w;
    std::vector<int> closest_on_path =
        modified_bfs(w, G, dist[s], visited[s], eccentricity, Ns_w, s - 1, N);
    eccentricity[w] = *std::max_element(dist[s].begin(), dist[s].end());

    for (int i = 0; i < s; i++) {
        std::fill(dist[i].begin(), dist[i].end(), -1);
        std::fill(visited[i].begin(), visited[i].end(), false);
    }

    std::map<int, int> key;
    int root = 0;
    for (auto &i : Ns_w) {
        found[i] = true;
        key[i] = root;
        bfs(i, root++, G, dist, visited, eccentricity, false);
        eccentricity[i] = *std::max_element(dist[i].begin(), dist[i].end());
    }

    for (int i = 0; i < N; i++) {
        if (G.in_LWCC(i) && !found[i]) {
            int vt = closest_on_path[i];
            if (dist[key[vt]][i] <= dist[s][vt]) {
                eccentricity[i] = std::max(eccentricity[i], eccentricity[vt]);
            } else {
                eccentricity[i] = std::max(eccentricity[i], min_ecc);
            }
        }
    }
    return eccentricity;
}
} // namespace our;

#endif