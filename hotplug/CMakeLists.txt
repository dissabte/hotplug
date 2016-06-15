cmake_minimum_required(VERSION 3.0)

set(LIBRARY_NAME hotplug)

message(STATUS "Processing ${LIBRARY_NAME}...")

project(${LIBRARY_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HOTPLUG_INC_DIR "include")
set(HOTPLUG_SRC_DIR "src")

file(GLOB_RECURSE HOTPLUG_INCLUDE_FILES "${HOTPLUG_INC_DIR}/*.h")
file(GLOB_RECURSE HOTPLUG_SOURCE_FILES "${HOTPLUG_SRC_DIR}/*.cpp")
file(GLOB_RECURSE HOTPLUG_PRIVATE_INCLUDE_FILES "${HOTPLUG_SRC_DIR}/*.h")

include_directories(${HOTPLUG_INC_DIR})

set(HOTPLUG_SOURCES ${HOTPLUG_INCLUDE_FILES} ${HOTPLUG_PRIVATE_INCLUDE_FILES} ${HOTPLUG_SOURCE_FILES})

# Unit tests
if(NOT UNITTEST_CPP_FOUND)
	include(../external/unittest-cpp.cmake)
endif()

add_library (${LIBRARY_NAME} ${HOTPLUG_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${HOTPLUG_INC_DIR})

target_link_libraries(${LIBRARY_NAME} -ludev -lpthread)

# Unit tests
if(UNITTEST_CPP_FOUND)
	add_subdirectory("tests")
endif()

# demo app
add_subdirectory("demo/")

# Doxygen
#set(HOTPLUG_DOCUMENTATION_DIR "docs")
#include(${HOTPLUG_DOCUMENTATION_DIR}/doxygen.cmake)

message(STATUS "Processing ${LIBRARY_NAME} done")
