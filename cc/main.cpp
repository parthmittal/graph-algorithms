#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

/* headers from ../common/include */
#include <ed-dfs.hpp>
#include <graph.hpp>
#include <graph_reader.hpp>
#include <optparser.hpp>
#include <reduced_graph.hpp>

/* headers from ./include */
#include <bounding_eccentricities.hpp>
#include <k_bfs.hpp>
#include <rv.hpp>

using namespace std;

int main(int argc, const char **argv) {
    our::OptParser parser;
    parser.addOption("-v", "--vertex_count");
    parser.addOption("-d", "--is_directed");
    parser.addOption("-p", "--to_prune");
    parser.addOption("-a", "--strategy_1");
    parser.addOption("-b", "--strategy_2");
    parser.addOption("-c", "--strategy_3");
    parser.addOption("-B", "--bounding_eccentricities");
    parser.addOption("-K", "--k_bfs");
    parser.addOption("-R", "--rv_algorithm");

    parser.parse(argc, argv);
    auto graph = our::read_file(
        "/home/kushagra/Desktop/6/IP/graph-algorithms/Email-Enron.txt", parser);

    our::naive_LWCC_t<int> naive_graph(graph.adj);
    our::LWCC_t<int> main_graph(graph.adj);
    our::graph_t<int> temp_graph(graph.adj);
    our::reduced_LWCC_t reduced_graph(
        temp_graph, our::ed_dfs::two_connected_prop(temp_graph));

    double time_taken;
    clock_t tStart;

    /* the bounding eccentricities algorithm */
    if (parser.checkIncluded("bounding_eccentricities")) {
        cerr << "--------------------BOUNDING "
                "ECCENTIRITIES-----------------------"
             << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "Graph type : Naive Graph" << endl;
        tStart = clock();
        vector<int> b_eccentricity_naive;
        if (parser.checkIncluded("strategy_1"))
            b_eccentricity_naive = our::bounding_eccentricities(
                naive_graph, naive_graph.N, 1, parser);
        else if (parser.checkIncluded("strategy_2"))
            b_eccentricity_naive = our::bounding_eccentricities(
                naive_graph, naive_graph.N, 2, parser);
        else if (parser.checkIncluded("strategy_3"))
            b_eccentricity_naive = our::bounding_eccentricities(
                naive_graph, naive_graph.N, 3, parser);
        //        for (auto &i : b_eccentricity_naive)
        //            cout << i << " ";
        //        cout << endl;
        time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "Graph type : CRS Graph" << endl;
        tStart = clock();
        vector<int> b_eccentricity_crs;
        if (parser.checkIncluded("strategy_1"))
            b_eccentricity_crs = our::bounding_eccentricities(
                main_graph, main_graph.N, 1, parser);
        else if (parser.checkIncluded("strategy_2"))
            b_eccentricity_crs = our::bounding_eccentricities(
                main_graph, main_graph.N, 2, parser);
        else if (parser.checkIncluded("strategy_3"))
            b_eccentricity_crs = our::bounding_eccentricities(
                main_graph, main_graph.N, 3, parser);
        time_taken = (clock() - tStart) / (double)(double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "--------------------------------------------------------------"
                "---"
             << endl;

        cerr << "Graph type : Reduced Graph : Biconnected component" << endl;
        tStart = clock();
        vector<int> b_eccentricity_reduced;
        if (parser.checkIncluded("strategy_1"))
            b_eccentricity_reduced = our::bounding_eccentricities(
                reduced_graph, reduced_graph.N, 1, parser);
        else if (parser.checkIncluded("strategy_2"))
            b_eccentricity_reduced = our::bounding_eccentricities(
                reduced_graph, reduced_graph.N, 2, parser);
        else if (parser.checkIncluded("strategy_3"))
            b_eccentricity_reduced = our::bounding_eccentricities(
                reduced_graph, reduced_graph.N, 3, parser);
        time_taken = (clock() - tStart) / (double)(double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "--------------------------------------------------------------"
                "---"
             << endl;
    }

    /*  the k_bfs algorithm with k = 60*/
    if (parser.checkIncluded("k_bfs")) {
        cerr << "-----------------------------K "
                "BFS-------------------------------"
             << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "Graph type : Naive Graph" << endl;
        tStart = clock();
        vector<int> kbfs_eccentricity_naive =
            our::k_bfs(naive_graph, 60, naive_graph.N);
        for (auto &i : kbfs_eccentricity_naive)
            cout << i << " ";
        cout << endl;
        time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "Graph type : CRS Graph" << endl;
        tStart = clock();
        vector<int> kbfs_eccentricity_crs =
            our::k_bfs(main_graph, 60, main_graph.N);
        time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "--------------------------------------------------------------"
                "---"
             << endl;
    }

    /*  the RV algorithm */
    if (parser.checkIncluded("rv_algorithm")) {
        cerr << "------------------------------RV------------------------------"
                "---"
             << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "Graph type : Naive Graph" << endl;
        int s = 2 * sqrt(naive_graph.N) * log(naive_graph.N);
        tStart = clock();
        vector<int> rv_eccentricity_naive =
            our::RV_algorithm(naive_graph, naive_graph.N, s);
        for (auto &i : rv_eccentricity_naive)
            cout << i << " ";
        cout << endl;
        time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "Graph type : CRS Graph" << endl;
        tStart = clock();
        vector<int> rv_eccentricity_crs =
            our::RV_algorithm(main_graph, main_graph.N, s);
        time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
        cerr << "Time taken = " << time_taken << endl;
        cerr << "=============================================================="
                "==="
             << endl;
        cerr << "--------------------------------------------------------------"
                "---"
             << endl;
    }

    // tStart = clock();
    // for(int i = 0; i < naive_graph.N; i++)
    // {
    //     if(naive_graph.in_LWCC(i))
    //        vector<int> D = our::ECCENTRICITY(naive_graph, i, naive_graph.N);
    // }
    // time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
    // cerr << "Time taken = " << time_taken << endl;
    // cerr << "=============================================================="
    //         "==="
    //      << endl;

    // tStart = clock();
    // for(int i = 0; i < main_graph.N; i++)
    // {
    //     if(main_graph.in_LWCC(i))
    //         vector<int> D = our::ECCENTRICITY(main_graph, i, main_graph.N);
    // }
    // time_taken = (clock() - tStart) / (double)CLOCKS_PER_SEC;
    // cerr << "Time taken = " << time_taken << endl;
    // cerr << "=============================================================="
    //         "==="
    //      << endl;
    return 0;
}
