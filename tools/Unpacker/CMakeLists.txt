add_executable(GCMUnpacker GCMUnpacker.cpp)
target_link_libraries(GCMUnpacker Unpacker Parser Disassembler cxxopts Logger)
