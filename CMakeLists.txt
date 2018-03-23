cmake_minimum_required(VERSION 3.9)
project(voa)

set(CMAKE_CXX_STANDARD 11)
#set the default path for built executables to the "bin" directory
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(/usr/include/curl)
include_directories(/usr/local/include)
include_directories(~/Codelabs/gumbo-parser/src)
include_directories(src)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib64)

add_executable(voa
        src/args.hxx
        src/voa.cpp
        src/voa_common.cpp
        src/voa_common.h
        src/voa_curl.cpp
        src/voa_curl.h
        src/voa_html.cpp
        src/voa_html.h)

TARGET_LINK_LIBRARIES(voa curl gumbo)
