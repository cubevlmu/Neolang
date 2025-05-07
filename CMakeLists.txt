cmake_minimum_required(VERSION 3.20)

project(ProjectNeon)

# libs
include(${CMAKE_SOURCE_DIR}/libs/index.cmake)

# projects
add_subdirectory(${CMAKE_SOURCE_DIR}/modules/NeoCompiler)