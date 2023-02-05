cmake_minimum_required(VERSION 3.0.0)
project(DOLPatcher VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(lib)

add_executable(DOLPatcher DOLPatcher.cpp)

