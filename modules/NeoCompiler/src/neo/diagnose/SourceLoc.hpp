#pragma once

#include "neo/common.hpp"
#include <string>

namespace neo {

    struct SourceLoc
    {
        psize line;
        psize column;
        class NSourceFile* file;

        std::string toString() const;
        void write(class NSerializer*) const;
        void read(class NSerializer*) const;
    };
    
}