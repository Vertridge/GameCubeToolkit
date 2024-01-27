add_subdirectory(ImGuiObjects)

add_library(GUIProgram GUIProgram.cpp)

target_link_libraries(GUIProgram GUILib Platform ImGuiObjs Logger)
