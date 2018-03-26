#include <set>
#include <vector>

#ifndef __RV_hpp__
#define __RV_hpp__

namespace our {
using namespace std;
template <typename graph_type>
void bfs(int source, int root, const graph_type &G, vector<vector<int>> &dist,
         vector<vector<bool>> &visited, vector<int> &eccentricity,
         bool update_eccentricity);

template <typename graph_type>
vector<int> modified_bfs(int source, const graph_type &G, vector<int> &dist,
                         vector<bool> &visited, vector<int> &eccentricity,
                         set<int> &Ns_w, int s, int N);

template <typename graph_type>
vector<int> RV_algorithm(const graph_type &G, int N, int s);
} // namespace our

#endif