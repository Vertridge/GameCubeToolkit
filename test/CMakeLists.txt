enable_testing(true)

# Make sure test end up in the test dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

include_directories(include)

add_custom_target(unittests)

add_subdirectory(Parser)
add_subdirectory(Disassembler)

add_custom_target(run_unittests
				  DEPENDS unittests
				  COMMAND ctest --output-on-faulure)
