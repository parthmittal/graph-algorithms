#include <algorithm>
#include <cstdlib>
#include <queue>
#include <set>
#include <vector>

#include <graph.hpp>

#ifndef __K_BFS_HPP__
#define __K_BFS_HPP__

namespace our {
typedef unsigned __int128 int128;
/*
 * A standard max_dist_bfs implementation which updates the dist vector so as to
 * find the
 * max distance from a set of vertices
 * source is the source of the max_dist_bfs
 * root represents the kth time for which this max_dist_bfs is run
 * dist is vector of pairs where second value represents the distance from the
 * current node while the first value represents the max distance from the
 * sources from which max_dist_bfs is
 */
template <typename graph_type>
void max_dist_bfs(int source, int root, const graph_type &G,
                  std::vector<std::pair<int, int>> &dist,
                  std::vector<std::vector<int>> &visited) {
    dist[source].second = 0;
    dist[source].first = std::max(dist[source].first, dist[source].second);
    visited[root][source] = 1;
    std::queue<int> q;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &i : G[u]) {
            if (!(visited[root][i])) {
                dist[i].second = dist[u].second + 1;
                dist[i].first = std::max(dist[i].second, dist[i].first);
                visited[root][i] = 1;
                q.push(i);
            }
        }
    }
}

template <>
void max_dist_bfs<reduced_graph_t>(int source, int root,
                                   const reduced_graph_t &G,
                                   std::vector<std::pair<int, int>> &dist,
                                   std::vector<std::vector<int>> &visited) {
    const int inf = 1e9;
    for (int i = 0; i < dist.size(); i++)
        dist[i].second = inf;
    dist[source].second = 0;
    dist[source].first = std::max(dist[source].first, dist[source].second);

    typedef int vertex_t;
    typedef int weight_t;

    std::set<std::pair<weight_t, vertex_t>> q;
    q.insert({dist[source].second, source});
    while (!q.empty()) {
        weight_t ref_w = q.begin()->first;
        vertex_t u = q.begin()->second;
        q.erase(q.begin());
        for (auto &fat_edge : G[u]) {
            vertex_t v = fat_edge.v;
            weight_t w = fat_edge.weight;
            weight_t dist_through_u = ref_w + w;
            if (dist_through_u < dist[v].second) {
                q.erase({dist[v].second, v});
                dist[v].second = dist_through_u;
                dist[v].first = std::max(dist[v].second, dist[v].first);
                q.insert({dist[v].second, v});
            }
        }
    }
}

template <typename graph_type>
std::vector<int> k_bfs(const graph_type &G, int k, int N) {
    srand((int)time(0));
    const int inf = 1e9;
    std::vector<std::vector<int>> visited(k, std::vector<int>(N, 0));
    std::vector<std::pair<int, int>> dist(N, {0, inf});

    /* generate k distinct vertices */
    std::set<int> k_vertices;
    while (k_vertices.size() < k) {
        int a = (rand() % N);
        k_vertices.insert(a);
    }

    /* PHASE 1
     * run max_dist_bfs from each of the k_vertices and keep a track of max
     * distance from these vertices
     */
    int root = 0;
    for (auto &v : k_vertices) {
        max_dist_bfs<graph_type>(v, root++, G, dist, visited);
    }

    int max_distance = std::max_element(dist.begin(), dist.end())->first;

    /* find the set of farthest nodes from the initial random sets */
    std::vector<int> farthest;
    for (int i = 0; i < N; i++) {
        if (dist[i].first == max_distance) {
            farthest.push_back(i);
        }
    }

    /* PHASE 2
     * running max_dist_bfs from the set of farthest vertices and maintaining
     * max distance
     */
    for (int i = 0; i < k; i++) {
        std::fill(visited[i].begin(), visited[i].end(), 0);
    }
    root = 0;
    for (auto &i : farthest) {
        if (root == k)
            break;
        max_dist_bfs(i, root++, G, dist, visited);
    }

    /* Setting the eccentricity of u to the max for v belonging to k_vertices,
     * farthest of d(v, u) */
    std::vector<int> eccentricity(N);
    for (int i = 0; i < N; i++) {
        eccentricity[i] = dist[i].first;
    }
    return eccentricity;
}

} //  namespace our
#endif
