message(STATUS "add submodule")
find_package(xtensor REQUIRED)
if(NOT xtensor_FOUND)
    message(FATAL_ERROR "xtensor not found. Please make sure xtensor is installed.")
else()
    message(STATUS "xtensor include directories: ${xtensor_INCLUDE_DIRS}")
endif()

find_package(xtl REQUIRED)
if(NOT xtl_FOUND)
    message(FATAL_ERROR "xtl not found. Please make sure xtl is installed.")
else()
    message(STATUS "xtl include directories: ${xtl_INCLUDE_DIRS}")
endif()

add_library(graph_lib INTERFACE)
target_include_directories(graph_lib INTERFACE ${XTENSOR_INCLUDE_DIRS} 
                                                ${XTL_INCLUDE_DIRS})