cmake_minimum_required(VERSION 3.22)
project(IMS)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

# Include directories
include_directories(${PROJECT_SOURCE_DIR})

# Source files
set(SOURCES ims.cpp simulation)

# Add the executable target
add_executable(simulation ${SOURCES})

# Link the Simlib library
find_library(SIMLIB_LIBRARY simlib REQUIRED)
if(SIMLIB_LIBRARY)
    message(STATUS "Simlib library found: ${SIMLIB_LIBRARY}")
else()
    message(FATAL_ERROR "Simlib library not found!")
endif()

target_link_libraries(simulation ${SIMLIB_LIBRARY})
