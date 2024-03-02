add_library(ImGuiObjs UnpackerGui.cpp 
                      LoggerGui.cpp 
                      PackageViewerGui.cpp
                      FileViewers/DOLViewerGui.cpp
                      FileViewers/FileViewerBase.cpp
                      FileViewers/TxtViewerGui.cpp)
target_link_libraries(ImGuiObjs ImGuiLib GUILib Unpacker Logger)
