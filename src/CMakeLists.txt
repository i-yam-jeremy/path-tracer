set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

set(OCL_ROOT "C:/Program Files (x86)/OCL_SDK_Light")
include_directories("${OCL_ROOT}/include")
link_directories("${OCL_ROOT}/lib/x86_64")

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} opencl)

add_library(${BINARY}_lib STATIC ${SOURCES})