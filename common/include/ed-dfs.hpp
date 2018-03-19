#include <utility>
#include <vector>

#include <graph.hpp>

#ifndef __EAR_DECOMPOSITION_DFS_hpp__
#define __EAR_DECOMPOSITION_DFS_hpp__

namespace our {
namespace ed_dfs {

typedef std::pair<int, int> edge_t;
typedef std::vector<edge_t> path_t;

class two_connected_prop {
  public:
    graph_t<int> G;
    std::vector<int> visited;
    std::vector<int> parent;
    std::vector<std::vector<int>> back_edges;
    std::vector<int> entry_time, exit_time;
    std::vector<int> vert;

    std::vector<int> visited_ear_decomposition;
    std::vector<std::vector<path_t>> ear_decomposition;
    /*
     * NB: path_t is a vector of edges.
     * So each element of ear_decomposition is a vector of ears,
     * corresponding to a 2-connected component.
     */
    std::vector<int> ears_of;

    two_connected_prop(const graph_t<int> &G);

    struct dfs_stack_elem_t {
        int u;
        std::vector<int>::const_iterator it;
    };

    void dfs(int root, int &current_time);

    int find_ears(int u, int parent_ears);

    void ear_decompose();
};

} // namespace ed_dfs
} // namespace our

#endif // __EAR_DECOMPOSITION_DFS_hpp__
