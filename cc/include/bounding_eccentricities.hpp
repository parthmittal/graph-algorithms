#include <optparser.hpp>
#include <vector>

#ifndef __BOUNDING_ECCENTRICITY_HPP__
#define __BOUNDING_ECCENTRICITY_HPP__

namespace our {
using namespace std;
template <typename graph_type>
vector<int> ECCENTRICITY(const graph_type &G, int source, int N);

template <typename graph_type>
vector<int> pruning(const graph_type &G, int N, int &count);

template <typename graph_type>
int select_from(int STRATEGY, const vector<bool> &is_candidate,
                const graph_type &G, const vector<int> &lower,
                const vector<int> &upper, const vector<int> &d, bool &high,
                int N);

template <typename graph_type>
vector<int> bounding_eccentricities(const graph_type &G, int N, int STRATEGY,
                                    OptParser &parser);
} // namespace our

#endif