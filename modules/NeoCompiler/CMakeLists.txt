project(NeoCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJC_STANDARD 17)

set(INCS 
    "${CMAKE_SOURCE_DIR}/modules/NeoCompiler/src"
)
set(LNKS
    ""
)
set(DEPS 
    ""
)
set(HEAD 
    ""
)

InstallFLib(INCS LNKS)
InstallSpdlog(INCS LNKS)

file(GLOB_RECURSE src
    "src/*.m"
    "src/*.cpp"
    "src/*.mm"
    "src/*.c"
)
add_executable(NeoCompiler
    ${src}
)
target_include_directories(NeoCompiler PRIVATE 
    ${INCS}
)
target_link_libraries(NeoCompiler PRIVATE 
    ${LNKS}
)
target_compile_definitions(NeoCompiler PRIVATE 
    _XOPEN_SOURCE
    _LIBCPP_DRIVERKIT
    ${HEAD}

    FLIB_EXTRA_CHECKS
    FLIB_ENABLE_SIMD
)