#include <vector>

#ifndef __K_BFS_HPP__
#define __K_BFS_HPP__

namespace our {
using namespace std;
template <typename graph_type>
void bfs(int source, int root, const graph_type &G,
         vector<pair<int, int>> &dist, vector<unsigned __int128> &visited);

template <typename graph_type>
vector<int> k_bfs(const graph_type &G, int k, int N);
} //  namespace our
#endif