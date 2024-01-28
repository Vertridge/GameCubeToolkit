add_library(ImGuiObjs UnpackerGui.cpp LoggerGui.cpp)
target_link_libraries(ImGuiObjs ImGuiLib GUILib Unpacker Logger)
