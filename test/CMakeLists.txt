# List your test source files here
set(TEST_SOURCES
    test_graph.cpp
    test_steiner.cpp
    test_tree.cpp
    test_ro.cpp
    # Add new test files here
)

# Loop through each source file to create an executable and link libraries
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the filename without extension (as test name)
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Add executable for each test
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Link libraries to each test executable
    target_link_libraries(${TEST_NAME} graph_lib)
endforeach()
