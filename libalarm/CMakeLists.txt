add_library(libalarm SHARED protocal.cpp json11/json11.cpp Base64.cpp md5.cpp session.cpp
AlarmCheck.cpp
Uart.cpp
Upss.cpp)

target_link_libraries(libalarm ${ExtLibs})
install (TARGETS libalarm DESTINATION bin)
install (FILES Uart.h session.h protocal.h DESTINATION include)
