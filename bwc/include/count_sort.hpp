#include <utility>
#include <vector>

#ifndef __COUNT_SORT_hpp__
#define __COUNT_SORT_hpp__

namespace our {
/* sorts by .second */
std::vector<std::pair<int, int>>
counting_sort(const std::vector<std::pair<int, int>> &inp);
}; // namespace our

#endif // __COUNT_SORT_hpp__

