# cmake minimum version required
cmake_minimum_required(VERSION 3.10)

# project name
project(WebImageCrawler VERSION 0.3)

# store all source files
file(GLOB NATIVE_SRCS src/*.cpp)

# project and its dependencies
add_executable(WebImageCrawler_make ${NATIVE_SRCS})

# asio
find_path(ASIOINCLUDE asio.hpp)

# boost
find_path(BOOSTBINDINCLUDE boost)

# include
target_include_directories(WebImageCrawler_make PUBLIC src/inc)
target_include_directories(WebImageCrawler_make PUBLIC ${ASIOINCLUDE})
target_include_directories(WebImageCrawler_make PUBLIC ${BOOSTBINDINCLUDE})
target_link_libraries(WebImageCrawler_make PUBLIC ssl)
target_link_libraries(WebImageCrawler_make PUBLIC crypto)

# set CXX standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
