#include <functional>
#include <iostream>
#include <queue>
#include <stack>

#include <graph.hpp>
#include <sssp.hpp>

namespace our {

struct queue_element_t {
    int dist, node;

    bool operator>(const queue_element_t &other) const {
        return (dist > other.dist);
    }
};

typedef long long ll;

void bfs(int source, const graph_t<int> &G, std::vector<int> &S,
        std::vector<int> &dist, std::vector<ll> &num_paths) {
    using namespace std;

    dist.resize(G.N, -1);
    num_paths.resize(G.N, 0);

    queue<int> bfq;

    bfq.push(source);
    dist[source] = 0; num_paths[source] = 1;

    while(!bfq.empty()) {
        int u = bfq.front();
        bfq.pop();
        S.push_back(u);

        for (int v : G[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                bfq.push(v);
            }

            if (dist[v] == dist[u] + 1) {
                num_paths[v] += num_paths[u];
            }
        }
    }

    reverse(S.begin(), S.end());
}

void sssp(int source, const reduced_graph_t &G, std::vector<int> &S,
          std::vector<int> &dist, std::vector<ll> &num_paths) {
    using namespace std;

    dist.resize(G.N, -1);
    num_paths.resize(G.N, 0);
    vector<int> popped(G.N, 0);

    priority_queue<queue_element_t, vector<queue_element_t>,
                   greater<queue_element_t>>
        dfq;

    dist[source] = 0;
    num_paths[source] = 1;
    dfq.push({0, source});

    while (!dfq.empty()) {
        auto curr = dfq.top();
        dfq.pop();

        /* have we relaxed edges leaving curr.node? */
        if (!popped[curr.node]) {
            popped[curr.node] = 1;
            S.push_back(curr.node);
            for (auto &e : G[curr.node]) {
                int targ = e.v;
                int weight = dist[curr.node] + e.weight;
                if (dist[targ] == -1 || weight < dist[targ]) {
                    dist[targ] = weight;
                    num_paths[targ] = 0;
                    dfq.push({weight, targ});
                }
                if (dist[targ] == weight) {
                    num_paths[targ] += num_paths[curr.node];
                }
            }
        }
    }
}
} // namespace our
