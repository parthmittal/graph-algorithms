#include <compress.hpp>
#include <fstream>
#include <graph.hpp>
#include <graph_reader.hpp>
#include <map>
#include <set>
#include <string>

namespace our {
naive_graph_t<int> read_file(std::string filename, OptParser parser) {
    std::vector<std::vector<int>> adj;
    int vertex_count = 0;
    int edge_count = 0;
    ifstream input_graph(filenme);
    if (input_graph.is_open()) {
        string text_line;
        if (parser.checkIncluded("vertex_count")) {
            std::getline(input_graph, text_line);
            std::istringstream iss(text_line);
            std::vector<std::string> splitted_line(
                std::isteam_iterator<std::string>{iss},
                std::istream_iterator<std::string>());
            vertex_count = std::stoi(splitted_line[0], nullptr);
            edge_count = std::stoi(splitted_line[1], nullptr);
        } else {
            std::set<int> vertex_list;
            std::vector<pair<int, int>> edge_list;
            while (std::getline(input_graph, text_line)) {
                std::istringstream iss(text_line);
                std::vector<std::string> splitted_line(
                    std::istream_iterator<std::string>{iss},
                    std::istream_iterator<std::string>());
                u = std::stoi(splitted_line[0], nullptr);
                v = std::stoi(splitted_line[1], nullptr);
                vertex_list.insert(u);
                vertex_list.insert(v);
                edge_list.push_back({u, v});
                edge_count += 1;
            }
        }
        int offset = 0;
        std::map<int, int> key = compress_list(vertex_list, offset);
        vertex_count = offset;
        adj.resize(vertex_count);
        for (auto &e : edge_list) {
            adj[key[e.first]].push_back(key[e.second]);
            if (!parser.checkIncluded("is_directed"))
                adj[key[e.second]].push_back(key[e.first]);
        }
        return { adj, vertex_count, edge_count }
    }
}
} // namespace our