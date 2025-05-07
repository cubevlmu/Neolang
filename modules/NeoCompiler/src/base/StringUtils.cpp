#include "StringUtils.hpp"

namespace neo {

    void replaceAll(std::string& str, const char* from, const char* to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, strlen(from), to);
            start_pos += strlen(to);
        }
    }
    
}