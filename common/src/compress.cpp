#include <compress.hpp>

namespace our {
std::map<int, int> compress_list(const std::set<int> &data, int &offset) {
    std::map<int, int> key;
    for (auto &i : data) {
        if (key.find(i) == key.end()) {
            key[i] = offset++;
        }
    }
    return key;
}
} // namespace our
