add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph graph_lib)

add_executable(test_st test_steiner.cpp)
target_link_libraries(test_st graph_lib)