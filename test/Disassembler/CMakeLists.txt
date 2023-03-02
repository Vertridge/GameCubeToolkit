set(TEST_FILES 
	main.cpp 
	TestPowerPC.cpp)

add_executable(test_disassembler ${TEST_FILES})

target_link_libraries(test_disassembler
					  Parser
					  Catch)

add_test(NAME test_disassembler COMMAND test_disassembler)

add_dependencies(unittests test_disassembler)
