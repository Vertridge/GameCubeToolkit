add_library(ImGuiObjs UnpackerGui.cpp 
                      LoggerGui.cpp 
                      PackageViewerGui.cpp
                      DOLDisassemblerGUI.cpp
                      FileViewers/DOLViewerGui.cpp
                      FileViewers/FileViewerBase.cpp
                      FileViewers/TxtViewerGui.cpp
                      FileViewers/BNRViewerGui.cpp)
target_link_libraries(ImGuiObjs ImGuiLib GUILib Unpacker Logger Disassembler)
