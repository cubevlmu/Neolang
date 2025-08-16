#include "SourceLoc.hpp"

#include "neo/compiler/SourceFile.hpp"
#include <filesystem>
#include <sstream>

namespace neo {

    std::string SourceLoc::toString() const
    {
        std::stringstream output {};
        std::filesystem::path p(file == nullptr ? "Unknown Source" : file->getPath());
        output << p.filename() << " [" << line << ':' << column << ']';

        return output.str();
    }

}