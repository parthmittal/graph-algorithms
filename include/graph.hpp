#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include <cassert>

#ifndef __GRAPH_hpp__
#define __GRAPH_hpp__

namespace our {

template<typename edge_t>
struct crs_row_t {
    const std::vector< edge_t > &crs;
    int si, ei;

    crs_row_t(const std::vector< edge_t > &crs, int si, int ei)
        : crs(crs), si(si), ei(ei)
    {
#ifdef DEBUG
        assert(0 <= si);
        assert(si <= ei);
        assert(ei < crs.size());
#endif
    }

    using iterator_t = typename std::vector< edge_t >::iterator;

    iterator_t begin() { return std::advance(crs.begin(), si); }

    iterator_t end() { return std::advance(crs.begin(), ei); }

    int size() { return std::distance(begin(), end()); }

    edge_t operator[](int idx)
    {
#ifdef DEBUG
        assert(idx < size());
#endif
        return *std::advance(begin(), idx);
    }
};

template<typename edge_t>
struct graph_t {
    int N; /* number of vertices */
    std::vector< edge_t > crs;
    std::vector< int > si, ei;

    graph_t (const std::vector< std::vector<edge_t> > &adj)
        : N(adj.size()), si(adj.size()), ei(adj.size())
    {
        for (int u = 0; u < N; ++u) {
            si[u] = crs.size();
            crs.insert(crs.end(), adj[u].begin(), adj[u].end());
            ei[u] = crs.size();
        }
    }

    crs_row_t< edge_t > operator[](int u)
    {
#ifdef DEBUG
        assert(0 <= u && u < N);
#endif
        return {crs, si[u], ei[u]};
    }
};

}

#endif
