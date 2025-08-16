#pragma once

#include "neo/common.hpp"
#include <string>

namespace neo {

    struct SourceLoc
    {
        u32 line;
        u32 column;
        class NSourceFile* file;

        std::string toString() const;
    };
    
}