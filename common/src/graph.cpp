#include <stack>

#include <graph.hpp>

template <typename edge_t> void our::LWCC_t<edge_t>::iterative_dfs(int source) {
    std::stack<int> S;
    while (!S.empty()) {
        int u = S.top();
        S.pop();
        if (visited[u])
            continue;
        visited[u] = true;
        n++;

        for (int i = graph_t<edge_t>::si[u]; i < graph_t<edge_t>::ei[u]; i++) {
            if (!visited[graph_t<edge_t>::crs[i]]) {
                S.push(graph_t<edge_t>::crs[i]);
            }
        }
    }
}

template <typename edge_t>
void our::naive_LWCC_t<edge_t>::iterative_dfs(int source) {
    std::stack<int> S;
    while (!S.empty()) {
        int u = S.top();
        S.pop();
        if (visited[u])
            continue;
        visited[u] = true;
        n++;

        for (int i = graph_t<edge_t>::si[u]; i < graph_t<edge_t>::ei[u]; i++) {
            if (!visited[graph_t<edge_t>::crs[i]]) {
                S.push(graph_t<edge_t>::crs[i]);
            }
        }
    }
}
