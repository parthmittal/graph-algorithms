#include <chrono>
#include <iomanip>
#include <iostream>

#include <brandes.hpp>
#include <bwc_our.hpp>
#include <ed-dfs.hpp>
#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>
#include <reduced_graph.hpp>

namespace our {
    OptParser config;
};

int main(int argc, const char **argv) {
    using namespace std;
    using namespace our;

	if (argc < 2) {
		cerr << "need filename to read graph from" << endl;
		exit(1);
	}

	const string filename = argv[argc - 1];

    config.addOption("-d", "--dry-run");
	config.addOption("-b", "--use-brandes");
    config.parse(argc - 1, argv);

    auto graph_tmp = read_file(filename, config);
	graph_t<int> graph = graph_t<int>(graph_tmp.adj);

//    ed_dfs::two_connected_prop ed_wrapper(graph);
//
//    /* iterate over biconnected components */
//    int cid = 1; /* component id */
//    for (auto &bcc : ed_wrapper.ear_decomposition) {
//        cout << "BCC #" << cid << endl;
//        int eid = 1; /* ear id within component */
//        for (auto &ear : bcc) {
//            cout << "    Ear #" << eid << '\n';
//            for (auto &edge : ear) {
//                cout << "        " << edge.first << " -> " << edge.second
//                     << '\n';
//            }
//            ++eid;
//        }
//        ++cid;
//    }
//
//    reduced_graph_t rgraph(graph, ed_wrapper);
//    for (int u = 0; u < rgraph.N; ++u) {
//        for (auto &e : rgraph[u]) {
//            printf("%d (%d)", rgraph.rid[u], u);
//            for (auto &w : e.vids) {
//                printf(" -- %d", w);
//            }
//            printf(" -- %d (%d)\n", rgraph.rid[e.v], e.v);
//        }
//    }


	cout << fixed << setprecision(8);
	if (config.checkIncluded("use-brandes")) {
		auto start = chrono::steady_clock::now();

		auto bwc = brandes::bwc_all(graph);

		auto finish = chrono::steady_clock::now();
		auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

		cerr << "computed betweenness centrality in"
			<< int_ms.count() << " milliseconds" << endl;

		for (int i = 0; i < graph.N; ++i) {
			cout << bwc[i] << '\n';
		}
	} else {
		auto start = chrono::steady_clock::now();

		bwc_our bc_wrapper(graph);
		bc_wrapper.sim_brandes_all();

		auto finish = chrono::steady_clock::now();
		auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

		cerr << "computed betweenness centrality in"
			<< int_ms.count() << " milliseconds" << endl;

		for (int i = 0; i < graph.N; ++i) {
			cout << bc_wrapper.bwc[i] << '\n';
		}
	}
}
