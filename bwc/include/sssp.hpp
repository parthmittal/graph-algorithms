#include <vector>

#include <reduced_graph.hpp>

#ifndef __SSSP_hpp__
#define __SSSP_hpp__

namespace our {
void bfs(int source, const graph_t<int> &G, std::vector<int> &S,
         std::vector<int> &dist, std::vector<long long> &num_paths);

void sssp(int source, const reduced_graph_t &G, std::vector<int> &S,
          std::vector<int> &dist, std::vector<long long> &num_paths);
};

#endif // __SSSP_hpp__
