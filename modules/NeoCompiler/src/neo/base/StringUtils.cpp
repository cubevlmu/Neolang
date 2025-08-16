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

    void splitStr(std::vector<std::string>& out, const std::string& str, char delim, bool skipEmpty) {
        size_t start = 0;
        size_t pos = 0;

        while ((pos = str.find(delim, start)) != std::string::npos) {
            if (pos > start || !skipEmpty) {
                out.emplace_back(str.substr(start, pos - start));
            }
            start = pos + 1;
        }

        if (start < str.size() || !skipEmpty) {
            out.emplace_back(str.substr(start));
        }
    }
}