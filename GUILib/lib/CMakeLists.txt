add_subdirectory(ImGui)

add_library(GUILib Window.cpp)
target_link_libraries(GUILib glfw OpenGL::GL glad IMGUI Logger)
