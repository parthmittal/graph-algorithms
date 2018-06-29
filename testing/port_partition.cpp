#include <iostream>
#include <map>
#include <string>

int main() {
    using namespace std;

    string line;
    int gc = -1;
    map<int, int> group;
    while (getline(cin, line)) {
        if (line[0] == 'G') {
            ++gc;
        } else {
            int v = stoi(line);
            group[v] = gc;
        }
    }

    for(auto &x : group) {
        cout << x.second << '\n';
    }
}
