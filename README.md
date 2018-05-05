# Closeness Centrality
---
### Compilation
For Debugging :
1. Go to Debug Folder : `cd Debug`
2. Compile all changes using make command : `make`

For Production code:
1. Go to the Release Folder : `cd Release`
2. Compile all changes using make command : `make`

### Running the program
`Release/cc/cc_exec <path_to_input_file> <option_list>

### Options
1. -v, --vertex_count : if the input file contains number of vertices and edges
2. -d, --is_directed : if the graph is connected
3. -p, --to_prune : if you want to prune the 1 degree vertices
4. -a, --strategy_1 : if you want to use strategy 1 for bounding eccentricites algorithm
5. -b, --strategy_2 : similar as above
6. -c, --strategy_3 : similar as above
7. -B, --bounding_eccentricities : if you want to run bounding eccentricities algorithm
8. -K, --k_bfs : if you want to run the K bfs algorithm algorithm
9. -R, --rv_algorithm : if you want to run the RV algorithm
