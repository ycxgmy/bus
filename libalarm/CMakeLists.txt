add_library(libalarm SHARED protocal.cpp json11/json11.cpp Base64.cpp md5.cpp session.cpp)

target_link_libraries(libalarm ${ExtLibs})
