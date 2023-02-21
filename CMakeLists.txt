cmake_minimum_required(VERSION 3.0.0)
project(DOLPatcher VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Find clang executable and set it as compiler
find_program(Clang_P clang++ REQUIRED True)
set(CMAKE_CXX_COMPILER ${Clang_P})

find_program(Clang_C clang REQUIRED True)
set(CMAKE_C_COMPILER ${Clang_C})

# Use clang lld instead of gnu ld
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")

# lots of warnings and all warnings as errors
add_compile_options(-Wall -Wextra -pedantic -Werror
    -Wno-unused-variable
)

# Enable clang ubsan when in debug mode
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Adding ubsan!")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=integer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=nullability")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=implicit-conversion")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=array-bounds")
    add_compile_options(-fno-omit-frame-pointer)
endif()

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(lib)

add_executable(DOLPatcher DOLPatcher.cpp)
target_link_libraries(DOLPatcher Parser Disassembler)

