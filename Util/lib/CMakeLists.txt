add_subdirectory(Logger)

add_library(Util MemoryStream.cpp FileUtil.cpp MemoryMap.cpp Error.cpp String.cpp)

target_link_libraries(Util Logger)
