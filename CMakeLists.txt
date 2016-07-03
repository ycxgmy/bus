cmake_minimum_required (VERSION 2.6)
project (bus)



find_package(OpenCV REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/libalarm")

SET(ExtLibs ${OpenCV_LIBS})

IF (UNIX)
# curl & curlpp
SET(ExtLibs ${ExtLibs} curl curlpp)

ELSEIF (WIN32)
include_directories("E:\\xrz.bus\\libcurl\\include" "E:\\xrz.bus\\libcurl\\include\\lib" "E:\\xrz.bus\\libcurl\\include\\include")
link_directories("E:\\xrz.bus\\libcurl\\lib")
add_definitions ("-DNOMINMAX")
SET(ExtLibs ${ExtLibs} libcurl.lib curlppMD.lib)

ELSEIF (APPLE)
ENDIF()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(libalarm)

add_executable(bus test.cpp)
target_link_libraries(bus ${ExtLibs} libalarm)
