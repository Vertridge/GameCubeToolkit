add_library(Parser DOL.cpp GCM.cpp FST.cpp BNR.cpp)

target_link_libraries(Parser Util Logger)
