#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

#include "count_sort.hpp"

namespace our {
typedef std::pair<int, int> pii;

std::vector<pii> counting_sort(const std::vector<pii> &inp) {
    using namespace std;

    /* we assume that each element is at most inp.size() */
    vector<int> C(inp.size() + 1);
    for (auto &x : inp) {
        C[x.second]++;
    }

    size_t tot = 0;
    for (size_t k = 0; k < C.size(); ++k) {
        int temp = C[k];
        C[k] = tot;
        tot += temp;
    }

    assert(tot == inp.size());

    vector<pii> out(inp.size());
    for (auto &x : inp) {
        out[C[x.second]++] = x;
    }

    return out;
}
} // namespace our
