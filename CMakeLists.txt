# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project(PathTracer)

set(CMAKE_CXX_STANDARD 14)

set(OCL_ROOT "C:/Program Files (x86)/OCL_SDK_Light")
include_directories("${OCL_ROOT}/include")
link_directories("${OCL_ROOT}/lib/x86_64")

add_custom_target(GenericParser DEPENDS ${Pomegranate-GENERATED_SRC})
add_custom_command(OUTPUT "${CMAKE_SOURCE_DIR}/src/render-cl-kernel.hpp"
	COMMAND
	python "${CMAKE_SOURCE_DIR}/build-scripts/cl-to-cpp-constant-converter.py" "${CMAKE_SOURCE_DIR}/src/render.cl" renderKernelCode > "${CMAKE_SOURCE_DIR}/src/render-cl-kernel.hpp"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	DEPENDS "${CMAKE_SOURCE_DIR}/src/render.cl"
	COMMENT "Copying OpenCL kernel to C++ header file"
)

include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE PathTracer_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set (flags_1 "-Wno-overloaded-virtual")
else()
  set (flags_1 "-MP /wd4251")
endif()

add_executable(PathTracer ${PathTracer_SRC})
target_link_libraries(PathTracer opencl)