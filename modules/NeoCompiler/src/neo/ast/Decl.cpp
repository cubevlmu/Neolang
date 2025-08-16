#include "Decl.hpp"

namespace neo {

    TopLevelDecls::~TopLevelDecls()
    {
        for (auto* ptr : decls) {
            delete ptr;
        }
        decls.clear();
    }

    ModuleDecl::~ModuleDecl()
    {
        delete children;
    }


    EnumDecl::~EnumDecl()
    {
        for (auto* ptr : children) {
            delete ptr;
        }
        children.clear();
    }


    InterfaceDecl::~InterfaceDecl()
    {
        for (auto* ptr : children) {
            delete ptr;
        }
        children.clear();
    }

}