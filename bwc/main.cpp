#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <brandes.hpp>
#include <brandes_sk.hpp>
#include <bwc_our.hpp>
#include <ed-dfs.hpp>
#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>
#include <reduced_graph.hpp>
#include <skeleton_sdm.hpp>

namespace our {
OptParser config;
}

int main(int argc, const char **argv) {
    using namespace std;
    using namespace our;

    config.addOption("-d", "--dry-run");
    config.addOption("-b", "--use-brandes");
    config.addOption("-s", "--insig-dijkstra");
    config.addOption("-p", "--brandes-pp");
    config.addOption("-v", "--verbose");
    config.addOption("-f", "--fast-interpolation");
    config.addOption("-j", "--joints-info");
    config.parse(argc, argv);

    auto graph_tmp = read_file(cin, config);
    graph_t<int> graph = graph_t<int>(graph_tmp.adj);

    cout << fixed << setprecision(8);
    auto start = chrono::steady_clock::now();
    auto finish = start;
    if (config.checkIncluded("use-brandes")) {
        if (config.checkIncluded("verbose")) {
            cerr << "computing BC using brandes' algorithm" << endl;
            cerr << "=================================================="
                 << endl;
        }

        auto bwc = brandes::bwc_all(graph);
        finish = chrono::steady_clock::now();

        for (int i = 0; i < graph.N; ++i) {
            cout << bwc[i] << '\n';
        }
    } else if (config.checkIncluded("brandes-pp")) {
        if (config.checkIncluded("verbose")) {
            cerr << "computing BC using brandes++ algorithm" << endl;
            cerr << "=================================================="
                 << endl;
        }

        vector<int> P = graph_tmp.P;

        sdm::sk_graph_t skeleton(graph, P);

        auto bwc = sdm::brandes_skall(skeleton);
        finish = chrono::steady_clock::now();

        for (int i = 0; i < graph.N; ++i) {
            cout << bwc[i] << '\n';
        }
    } else {
        if (config.checkIncluded("verbose")) {
            cerr << "computing BC using our algorithm" << endl;
            cerr << "=================================================="
                 << endl;
        }

        bwc_our bc_wrapper(graph);
        bc_wrapper.sim_brandes_all();
        finish = chrono::steady_clock::now();

        if (config.checkIncluded("d") || config.checkIncluded("j")) {
            /* don't print bwc, since we didn't actually compute it */
        } else {
            for (int i = 0; i < graph.N; ++i) {
                cout << bc_wrapper.bwc[i] << '\n';
            }
        }
    }

    auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

    if (config.checkIncluded("verbose")) {
        cerr << "computed betweenness centrality in " << int_ms.count()
             << " milliseconds" << endl;
        cerr << "==================================================" << endl;
    } else {
        cerr << int_ms.count() << endl;
    }
}
