cmake_minimum_required(VERSION 3.0.0)
project(GameCubeToolkit VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE "Debug")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

message("Building in ${CMAKE_BUILD_TYPE}")

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

    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)

    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O0")

else()
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(vendor)
include_directories(${CMAKE_SOURCE_DIR}/Util/include)
add_subdirectory(Util)
include_directories(${CMAKE_SOURCE_DIR}/GCToolkit/include)
add_subdirectory(GCToolkit)
include_directories(${CMAKE_SOURCE_DIR}/GUILib/include)
add_subdirectory(GUILib)
add_subdirectory(test)
include_directories(${CMAKE_SOURCE_DIR}/tools/GUI/include)
add_subdirectory(tools)



