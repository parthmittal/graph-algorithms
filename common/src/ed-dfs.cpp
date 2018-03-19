#include <ed-dfs.hpp>
#include <graph.hpp>

#include <cassert>
#include <chrono>
#include <iostream>
#include <stack>
#include <vector>

namespace our {
namespace ed_dfs {

two_connected_prop::two_connected_prop(const graph_t<int> &G)
    : G(G), visited(G.N), parent(G.N), back_edges(G.N), entry_time(G.N),
      exit_time(G.N), vert(G.N + 1), visited_ear_decomposition(G.N),
      ears_of(G.N) {}

void two_connected_prop::dfs(int root, int &current_time) {
    std::stack<dfs_stack_elem_t> dfs_stack;
    dfs_stack.push({root, G[root].begin()});
    visited[root] = 1;
    entry_time[root] = ++current_time;
    vert[current_time] = root;

    while (!dfs_stack.empty()) {
        int u = dfs_stack.top().u;
        std::vector<int>::const_iterator &it = dfs_stack.top().it;
        if (it == G[u].end()) {
            dfs_stack.pop();
            exit_time[u] = current_time;
        } else {
            int v = *it;
            if (!visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                entry_time[v] = ++current_time;
                vert[current_time] = v;
                dfs_stack.push({v, G[v].begin()});
            } else if (v != parent[u]) {
                if (entry_time[v] >= entry_time[u]) {
                    back_edges[u].push_back(v);
                }
            }
            ++it;
        }
    }
}

int two_connected_prop::find_ears(int u, int parent_ears) {
    int curr_ears;
    if (!visited_ear_decomposition[u]) {
        ear_decomposition.push_back({});
        curr_ears = ear_decomposition.end() - 1 - ear_decomposition.begin();
    } else {
        curr_ears = parent_ears;
    }

    for (int &v : back_edges[u]) {
        //		std::cerr << u << ' ' << v << std::endl;
        visited_ear_decomposition[u] = 1;
        path_t ear;
        int curr = v, prev = u;
        while (!visited_ear_decomposition[curr]) { // break on first vertex
                                                   // already visited.
            ear.push_back({prev, curr});
            visited_ear_decomposition[curr] = 1;
            prev = curr;
            curr = parent[curr];
        }
        // still have to push edge we used to get to first already
        // visited vertex to ear.
        ear.push_back({prev, curr});
        ear_decomposition[curr_ears].push_back(ear);
    }

    return curr_ears;
}

void two_connected_prop::ear_decompose() {
    int total_back_edges = 0;
    int current_time = 0;
    for (int i = 0; i < G.N; ++i) {
        if (!visited[i]) {
            dfs(i, current_time);
        }
        total_back_edges += back_edges[i].size();
    }
#ifdef DEBUG
    assert(current_time == G.N);
#endif

    ear_decomposition.push_back({});
    ears_of[vert[1]] = find_ears(vert[1], 0);
    for (int i = 2; i <= current_time; ++i) {
        int u = vert[i];
        int p = parent[u];
        ears_of[u] = find_ears(u, ears_of[p]);
    }

    ear_decomposition.erase(ear_decomposition.begin());
}
} // namespace ed_dfs
} // namespace our
