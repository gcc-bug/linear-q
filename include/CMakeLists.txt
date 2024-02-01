add_library(graph_lib INTERFACE)
target_include_directories(graph_lib INTERFACE ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS})