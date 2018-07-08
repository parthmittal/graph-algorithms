#include <iostream>
#include <set>
#include <utility>

int main()
{
    using namespace std;

    int N, M;
    cin >> N >> M;

    typedef pair<int, int> edge_t;
    set<edge_t> edges;

    for (int i = 1; i <= M; ++i) {
        int x, y;
        cin >> x >> y;
        if (x > y) {
            swap(x, y);
        }
        edges.insert({x, y});
    }

    cout << N << ' ' << edges.size() << endl;
    for (auto &e : edges) {
        cout << e.first << ' ' << e.second << '\n';
    }
}
