#include <map>
#include <set>
#include <vector>

#ifndef __COMPRESS_hpp__
#define __COMPRESS_hpp__

namespace our {
std::map<int, int> compress_list(const std::set<int> &data, int &offset);
} // namespace our

#endif