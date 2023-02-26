add_library(Disassembler PowerPC.cpp Program.cpp)

find_library(CAPSTONE_LIB capstone)
message("Found capstone at: ${CAPSTONE_LIB}")

target_link_libraries(Disassembler ${CAPSTONE_LIB})

