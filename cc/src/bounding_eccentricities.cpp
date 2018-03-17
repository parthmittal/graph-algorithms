#include <graph.hpp>
#include <queue>
#include <vector>

using namespace our;

/* an iterative bfs implementation that returns the distance vector from the
 * source */
template <typename edge_t>
std::vector<int> ECCENTRICITY(const graph_t<edge_t> &G, int source, int N) {
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
    return dist;
}

