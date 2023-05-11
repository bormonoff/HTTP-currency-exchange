#include "util/util.h"

namespace util {

bool IsDouble(std::string& str) {
    size_t point_count;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '.') { 
            ++point_count; 
            if (point_count == 2) {return false; }
            continue;
        }
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    } 
    return true;
}
}  // namespace util
