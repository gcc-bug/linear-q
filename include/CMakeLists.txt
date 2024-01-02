add_library(graph_lib INTERFACE)
target_include_directories(graph_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/graph ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS})