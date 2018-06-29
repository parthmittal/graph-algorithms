#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <random>
#include <stack>
#include <vector>

#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>

using namespace std;
using namespace our;

typedef pair<int, int> pii;
const int max_N = 5e6 + 10;

int vis[max_N], ent[max_N], low[max_N], p[max_N];
stack<pii> S;
vector<vector<pii>> bcc;

void dfs(const naive_graph_t<int> &graph, int node, int &timer);
void store_bcc(int u, int v);

int main(int argc, const char **argv) {
    using namespace std;

    if (argc < 2) {
        fprintf(stderr, "need abs path to path to read graph from\n");
        exit(0);
    }

    const string filename = argv[1];

    our::OptParser parser;

    auto graph = our::read_file(filename, parser);

    if (graph.N > max_N) {
        fprintf(stderr, "graph too big, I too lazy\n");
        exit(0);
    }

    int timer = 1;
    for (int i = 0; i < graph.N; ++i) {
        if (!vis[i]) {
            dfs(graph, i, timer);
        }
    }

    vector<pii> all_edges;
    for (auto &B : bcc) {
        if (B.size() > all_edges.size()) {
            all_edges = B;
        }
    }

    /* count number of vertices */
    map<int, int> vert;
    for (auto &e : all_edges) {
        vert[e.first];
        vert[e.second];
    }

    printf("%lu %lu\n", vert.size(), all_edges.size());
    for (auto &e : all_edges) {
        printf("%d %d\n", e.first, e.second);
    }
}

void dfs(const naive_graph_t<int> &graph, int node, int &timer) {
    vis[node] = 1;
    low[node] = ent[node] = timer++;

    for (auto &v : graph.adj[node]) {
        if (!vis[v]) { /* tree edge */
            S.push({node, v});
            p[v] = node;
            dfs(graph, v, timer);
            low[node] = min(low[node], low[v]);
            if (low[v] >= ent[node]) {
                store_bcc(node, v);
            }
        } else if (v != p[node] && ent[v] <= ent[node]) { /* back edge */
            low[node] = min(low[node], ent[v]);
            S.push({node, v});
        }
    }
}

void store_bcc(int u, int v) {
    vector<pii> temp;
    while (S.top() != make_pair(u, v)) {
        temp.push_back(S.top());
        S.pop();
    }
    temp.push_back(S.top());
    S.pop();
    bcc.push_back(temp);
}
