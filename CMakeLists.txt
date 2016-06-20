cmake_minimum_required (VERSION 2.6)
project (test)
find_package(OpenCV REQUIRED)
include_directories("json11")
include_directories("${PROJECT_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(test test.cpp protocal.cpp json11/json11.cpp Base64.cpp md5.cpp)
target_link_libraries(test ${OpenCV_LIBS}
  curl curlpp
  )
