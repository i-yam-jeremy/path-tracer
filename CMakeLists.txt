# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project(PathTracer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)

include_directories(src)

add_subdirectory(src)
add_subdirectory("test")
add_subdirectory(lib/googletest)