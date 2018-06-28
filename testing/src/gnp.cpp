#include <random>
#include <utility>
#include <vector>

#include <gnp.hpp>

namespace gen {
std::vector<std::pair<int, int>> gnp(int N, double p) {
    using namespace std;
    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);

    /* This is \Omega(N^2); but it is possible to do better */
    vector<pair<int, int>> edges;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (dis(gen) <= p) {
                edges.push_back({i, j});
            }
        }
    }

    return edges;
}
}; // namespace gen
