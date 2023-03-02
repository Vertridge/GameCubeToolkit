set(TEST_FILES 
	main.cpp 
	TestDOL.cpp)

add_executable(test_parser ${TEST_FILES})

target_link_libraries(test_parser
					  Parser
            Util
					  Catch)

add_test(NAME test_parser COMMAND test_parser)

add_dependencies(unittests test_parser)
