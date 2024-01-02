cmake_minimum_required(VERSION 3.10) 
project("linear-q") 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
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

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(include)
add_subdirectory(test)

# Detect operating system and set OS-specific settings
if(WIN32)
    # Windows-specific settings
    # Example: Define a preprocessor macro for Windows
    add_definitions(-DWINDOWS)

    # Link Windows-specific libraries if needed
    # Example: Uncomment and add necessary libraries
    # target_link_libraries(linear-q wsock32 ws2_32)
elseif(APPLE)
    # macOS-specific settings
    # Example: Define a preprocessor macro for macOS
    add_definitions(-DMACOS)

    # macOS-specific configurations
    # Example: Uncomment and add necessary configurations
    # set(MACOSX_BUNDLE_ICON_FILE myicon.icns)
    # set(APP_TYPE MACOSX_BUNDLE)
elseif(UNIX)
    # Linux/Unix-specific settings
    # Example: Define a preprocessor macro for Unix/Linux
    add_definitions(-DLINUX)

    # Link Unix/Linux-specific libraries if needed
    # Example: Uncomment and add necessary libraries
    # target_link_libraries(linear-q pthread)
endif()

# Compiler options can also be set conditionally
if(MSVC)
    # MSVC-specific compiler options
    add_compile_options(/W3)
else()
    # GCC/Clang-specific compiler options
    add_compile_options(-Wall -Wextra -pedantic)
endif()