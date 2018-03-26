#include <iostream>

/* headers from ../include */
#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>

/* headers from ./include */

using namespace std;

int main(int argc, const char **argv) {
    our::OptParser parser;
    parser.addOption("-v", "--vertex_count");
    parser.addOption("-d", "--is_directed");
    parser.addOption("-a", "--some_random_option");
    parser.parse(argc, argv);
    int v = parser.checkIncluded("vertex_count");
    int d = parser.checkIncluded("is_directed");
    int a = parser.checkIncluded("a");
    auto graph = our::read_file(
        "/home/kushagra/Desktop/6/IP/graph-algorithms/Email-Enron.txt", parser);
    cout << "Vertex count in Enron: " << graph.vertex_count << endl;
    return 0;
}
