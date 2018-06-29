#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>

#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>

/*
 * INPUT:  undirected, unweighted graph in edge list format
 * OUTPUT: same graph in adjacency list format (0/1 indexed by runtime arg)
 */

int main(int argc, char *argv[])
{
    using namespace std;
    using namespace our;

    int j = 1;
    if (argc == 2) {
        j = stoi(argv[1]);
    }

    OptParser temp;
    auto graph = read_file(cin, temp);

    int N = graph.N;
    int M = 0;

    for (auto &x : graph.adj) {
        M += x.size();
    }
    M /= 2;

    cout << N << ' ' << M << endl;
    for (int u = 0; u < N; ++u) {
        for (int v : graph.adj[u]) {
            cout << v + j << ' ';
        }
        cout << '\n';
    }
}
