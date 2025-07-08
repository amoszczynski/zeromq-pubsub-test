cmake_minimum_required(VERSION 3.20)
project(zeromq-test)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find and link ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Find cppzmq (C++ bindings)
find_path(CPPZMQ_INCLUDE_DIR zmq.hpp)
if(NOT CPPZMQ_INCLUDE_DIR)
    message(FATAL_ERROR "cppzmq not found. Install with: brew install cppzmq")
endif()

target_link_libraries(${PROJECT_NAME} ${ZMQ_LIBRARIES})
target_link_directories(${PROJECT_NAME} PRIVATE ${ZMQ_LIBRARY_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${ZMQ_INCLUDE_DIRS} 
    ${CPPZMQ_INCLUDE_DIR}
    /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1
)
target_compile_options(${PROJECT_NAME} PRIVATE ${ZMQ_CFLAGS_OTHER} -stdlib=libc++)

# Optional: Set compiler flags for better debugging and warnings
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
