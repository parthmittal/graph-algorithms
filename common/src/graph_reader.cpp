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
naive_graph_t<int> read_file(const std::string &filename, OptParser &parser) {
    /* it's safe to use namespace within a function */
    using namespace std;

    int vertex_count = 0;
    int edge_count = 0;
    cerr << "Filename: " << filename << endl;
    ifstream input_graph(filename);
    if (input_graph.is_open()) {
        string text_line;
        if (parser.checkIncluded("vertex_count")) {
            input_graph >> vertex_count >> edge_count;
        }

        set<int> vertex_list;
        vector<pair<int, int>> edge_list;

        int u, v;
        while (input_graph >> u >> v) {
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
            if (!parser.checkIncluded("is_directed"))
                adj[key[e.second]].push_back(key[e.first]);
        }
        return {adj};
    } else {
        cerr << "File does not open. Check filename provided.\n";
    }
}
} // namespace our
