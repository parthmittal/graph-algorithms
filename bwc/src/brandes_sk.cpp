#include <queue>
#include <vector>

#include <graph.hpp>
#include <skeleton_sdm.hpp>

namespace sdm {
struct queue_element_t {
    int dist, v;

    bool operator>(const queue_element_t &other) const {
        return (dist > other.dist);
    }
};

std::vector<double> brandes_skall(const sk_graph_t<int> &G,
                                  std::vector<int> dest,
                                  std::vector<double> &bc) {
    using namespace std;
    stack<int> S;

    for (int i = 0; i < part.size(); ++i) {
        for (int j = 0; j < part.size(); ++j) {
        }
    }

    vector<int> dist(G.N, -1);
    vector<int> num_paths(G.N, 0);
    vector<int> popped(G.N, 0);

    priority_queue<queue_element_t, vector<queue_element_t>,
                   greater<queue_element_t>>
        dfq;

    dist[source] = 0;
    num_paths[source] = 1;
    dfq.push({0, -1, source});

    while (!dfq.empty()) {
        auto curr = dfq.top();
        dfq.pop();

        if (!popped[curr.v]) {
            popped[curr.v] = 1;
            S.push_back(curr.v);
            for (auto &e : G[curr.v]) {
                int targ = e.v;
                int weight = dist[curr.v] + e.weight;
                if (dist[targ] == -1 || weight < dist[targ]) {
                    dist[targ] = weight;
                    num_paths[targ] = 0;
                    dfq.push({weight, curr.node, targ});
                }
                if (weight == dist[targ]) {
                    num_paths[targ] += num_paths[v] * e.mult;
                }
            }
        }
    }

    vector<double> delta(G.N);
}
} // namespace sdm
