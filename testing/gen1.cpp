#include <cstdlib>
#include <iostream>
#include <string>

#include <gnp.hpp>

int main(int argc, char *argv[]) {
    using namespace std;

    if (argc < 3) {
        cerr << "USAGE: num_vertices p" << endl;
        exit(1);
    }

    int N = stoi(argv[1]);
    double p = stod(argv[2]);

    auto edges = gen::gnp(N, p);

    cout << N << ' ' << edges.size() << endl;
    for (auto &e : edges) {
        cout << e.first << ' ' << e.second << '\n';
    }
}
