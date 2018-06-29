#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include <compress.hpp>
#include <graph.hpp>
#include <graph_reader.hpp>

namespace our {
naive_graph_t<int> read_file(std::istream &in, OptParser &parser) {
    /* it's safe to use namespace within a function */
    using namespace std;

    int vertex_count = 0;
    int edge_count = 0;
    string text_line;
    if (parser.checkIncluded("no-count") != 1) {
        in >> vertex_count >> edge_count;
    }

    set<int> vertex_list;
    vector<pair<int, int>> edge_list;

    while (edge_count--) {
        int u, v;
        in >> u >> v;
        vertex_list.insert(u);
        vertex_list.insert(v);
        edge_list.push_back({u, v});
    }

    int offset = 0;
    map<int, int> key = compress_list(vertex_list, offset);
    vertex_count = vertex_list.size();
    vector<vector<int>> adj(vertex_count);
    for (auto &e : edge_list) {
        adj[key[e.first]].push_back(key[e.second]);
        if (!(parser.checkIncluded("is_directed") == 1))
            adj[key[e.second]].push_back(key[e.first]);
    }

    if (parser.checkIncluded("brandes-pp") == 1) {
        vector<int> P(vertex_count);
        for (int i = 0; i < vertex_count; ++i) {
            in >> P[i];
        }
        return {adj, P};
    } else {
        return {adj};
    }
}
} // namespace our
