add_subdirectory(lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(GUI GUI.cpp)
target_link_libraries(GUI GUIProgram cxxopts Platform)
