#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <utility>
#include <vector>

#include <cassert>

#ifndef __GRAPH_hpp__
#define __GRAPH_hpp__

namespace our {

template <typename edge_t> struct naive_graph_t {
    std::vector<std::vector<edge_t>> adj;
    std::vector<int> P;
    int N;

    naive_graph_t(const std::vector<std::vector<edge_t>> &adj)
        : adj(adj), N(adj.size()) {}

    naive_graph_t(const std::vector<std::vector<edge_t>> &adj,
                  std::vector<int> P)
        : adj(adj), P(P), N(adj.size()) {}

    const std::vector<edge_t> &operator[](int u) const {
#ifdef DEBUG
        assert(0 <= u && u < N);
#endif
        return {adj[u]};
    }

    naive_graph_t() : N(0) {}
};

template <typename edge_t> struct crs_row_t {
    const std::vector<edge_t> &crs;
    int si, ei;

    crs_row_t(const std::vector<edge_t> &crs, int si, int ei)
        : crs(crs), si(si), ei(ei) {
#ifdef DEBUG
        assert(0 <= si);
        assert(si <= ei);
        assert(ei < crs.size());
#endif
    }

    using iterator_t = typename std::vector<edge_t>::const_iterator;

    iterator_t begin() { return (crs.begin() + si); }

    iterator_t end() { return (crs.begin() + ei); }

    int size() { return std::distance(begin(), end()); }

    const edge_t operator[](int idx) {
#ifdef DEBUG
        assert(idx < size());
#endif
        return *(begin() + idx);
    }
};

template <typename edge_t> struct graph_t {
    int N; /* number of vertices */
    std::vector<edge_t> crs;
    std::vector<int> si, ei;

    graph_t(const std::vector<std::vector<edge_t>> &adj)
        : N(adj.size()), si(adj.size()), ei(adj.size()) {
        for (int u = 0; u < N; ++u) {
            si[u] = crs.size();
            crs.insert(crs.end(), adj[u].begin(), adj[u].end());
            ei[u] = crs.size();
        }
    }

    crs_row_t<edge_t> operator[](int u) const {
#ifdef DEBUG
        assert(0 <= u && u < N);
#endif
        return {crs, si[u], ei[u]};
    }
};

template <typename edge_t> struct LWCC_t : graph_t<edge_t> {
    bool in_LWCC(int i) const { return visited[i]; }
    int size_LWCC() const { return n; }

    LWCC_t(const std::vector<std::vector<edge_t>> &adj)
        : graph_t<edge_t>(adj), visited(adj.size(), 0), n(0) {

        int max_source = -1, max_count = 0;
        for (int i = 0; i < adj.size(); i++) {
            if (!visited[i]) {
                int temp_count = 0;
                iterative_dfs(i, temp_count);
                if (temp_count > max_count) {
                    max_source = i;
                    max_count = temp_count;
                }
            }
        }
        std::fill(visited.begin(), visited.end(), 0);
        iterative_dfs(max_source, n);

        std::cerr
            << "The number of nodes in the largest weakly connected component: "
            << n << std::endl;
    }

  private:
    std::vector<int> visited;
    int n; /* number of vertices in LWCC */
    void iterative_dfs(int source, int &comp_count) {
        std::stack<int> S;
        S.push(source);
        while (!S.empty()) {
            int u = S.top();
            S.pop();
            if (visited[u])
                continue;
            visited[u] = true;
            comp_count++;

            for (int i = graph_t<edge_t>::si[u]; i < graph_t<edge_t>::ei[u];
                 i++) {
                if (!visited[graph_t<edge_t>::crs[i]]) {
                    S.push(graph_t<edge_t>::crs[i]);
                }
            }
        }
    }
};

template <typename edge_t> struct naive_LWCC_t : naive_graph_t<edge_t> {
    bool in_LWCC(int i) const { return visited[i]; }
    int size_LWCC() const { return n; }

    naive_LWCC_t(const std::vector<std::vector<edge_t>> &adj)
        : naive_graph_t<edge_t>(adj), visited(adj.size(), false), n(0) {

        int max_source = -1, max_count = 0;
        for (int i = 0; i < adj.size(); i++) {
            if (!visited[i]) {
                int temp_count = 0;
                iterative_dfs(i, temp_count);
                if (temp_count > max_count) {
                    max_source = i;
                    max_count = temp_count;
                }
            }
        }
        std::fill(visited.begin(), visited.end(), 0);
        iterative_dfs(max_source, n);

        std::cerr << "The number of nodes in the largest weakly connected "
                     "component are : "
                  << n << std::endl;
    }

  private:
    std::vector<int> visited;
    int n;
    void iterative_dfs(int source, int &comp_count) {
        std::stack<int> S;
        S.push(source);
        while (!S.empty()) {
            int u = S.top();
            S.pop();
            if (visited[u])
                continue;
            visited[u] = true;
            comp_count++;

            for (int i = 0; i < naive_graph_t<edge_t>::adj[u].size(); i++) {
                if (!visited[naive_graph_t<edge_t>::adj[u][i]]) {
                    S.push(naive_graph_t<edge_t>::adj[u][i]);
                }
            }
        }
    }
};

} // namespace our

#endif
