cmake_minimum_required (VERSION 2.6)
project (bus)



find_package(OpenCV REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/libalarm")
include_directories("${PROJECT_SOURCE_DIR}/libalarm/json11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(libalarm)

add_executable(bus test.cpp)
target_link_libraries(bus ${OpenCV_LIBS} curl curlpp libalarm)
