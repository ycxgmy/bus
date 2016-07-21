#ifndef ___CONFIG__H_
#define ___CONFIG__H_
#ifdef WIN32
#define APIDEC __declspec(dllexport)
#define BUILDING_CURLPP
#endif
#define PATH_LOGIN  "http://uwonders.ticp.net:8089/alarmupload/login"
#define PATH_ALARM  "http://uwonders.ticp.net:8089/alarmupload/alarm"
#define PATH_CONNT  "http://uwonders.ticp.net:8089/alarmupload/connt"
#define PATH_UPLOAD_FILE_INFO  "http://uwonders.ticp.net:8088/upload_file_info"
#define PATH_UPLOAD_BIG_FILE  "http://uwonders.ticp.net:8088/upload_big_file"
#define PATH_UPLOAD_FILE  "http://uwonders.ticp.net:8088/upload_file"
#endif
