cmake_minimum_required (VERSION 2.6)
project (bus)



find_package(OpenCV REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/libalarm")

SET(ExtLibs "")

IF (UNIX)
# curl & curlpp
SET(ExtLibs ${ExtLibs} curl curlpp)

ELSEIF (WIN32)

ELSEIF (APPLE)
ENDIF()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(libalarm)

add_executable(bus test.cpp)
target_link_libraries(bus ${OpenCV_LIBS} ${ExtLibs} libalarm)
