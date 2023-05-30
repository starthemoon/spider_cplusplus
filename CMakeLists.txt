# cmake minimum version required
cmake_minimum_required(VERSION 3.10)

# project name
project(WebImageCrawler VERSION 0.3)

# store all source files
file(GLOB NATIVE_SRCS src/*.cpp)

# project and its dependencies
add_executable(WebImageCrawler_make ${NATIVE_SRCS})

# include
target_include_directories(WebImageCrawler_make PUBLIC src/inc)

# set CXX standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
