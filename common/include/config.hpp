#include <map>
#include <string>

#ifndef __CONFIG_hpp__
#define __CONFIG_hpp__

namespace our {
class config {
    std::map<std::string, int> enabled;

    void enable(std::string option);
};
}  // namespace our

#endif
