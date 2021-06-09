cmake_minimum_required(VERSION 3.5.1)

# Define variables:
# Project name.
set(PROJ_NAME PeopleDetector)
# Path to Darknet project folder.
set(DARKNET_PATH /home/neeekit/darknet_v4_may)
# Give a custom name to shared library which is provided by Darknet when the one compiled.
set(DARKNET_LIB_NAME libdarknet)
# Define C++ level, could be 11 or 17 as well.
set(CMAKE_CXX_STANDARD 14)
# Define compiler optimization level.
set(CMAKE_CXX_FLAGS "-O3")
# Do print warnings uppon compilation.
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# Apply flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(${PROJ_NAME})
file(GLOB_RECURSE SOURCE_FILES "src/*")
add_executable(${PROJ_NAME} ${SOURCE_FILES})

# Find and link OpenCV library.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find and link Boost library.
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# Link Darknet library' include folder.
include_directories(${DARKNET_PATH}/include)

# Headers file include folder.
include_directories(include)

# Define Darknet library' shared library.
add_library(${DARKNET_LIB_NAME} SHARED IMPORTED)

# Set a path to Darknet library' shared library.
set_property(TARGET ${DARKNET_LIB_NAME} PROPERTY IMPORTED_LOCATION "${DARKNET_PATH}/libdarknet.so")

# Link all libraries together.
target_link_libraries(${PROJ_NAME} ${DARKNET_LIB_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES})
