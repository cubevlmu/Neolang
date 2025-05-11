#pragma once

#include <string>
#include <sstream>

namespace neo {

    void replaceAll(std::string& str, const char* from, const char* to);

    template <typename... T>
    std::string msg(T&&... args) {
        std::ostringstream oss {};
        (oss << ... << args);
        return oss.str();
    }
}