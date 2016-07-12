#ifndef ___PROTOCAL__H_
#define ___PROTOCAL__H_

#include "config.h"
#include "Base64.h"
#include "md5.h"
#include "json11/json11.hpp"
using namespace json11;

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#include <future>

//#include <openssl/md5.h>
#define CURLPP
#ifdef CURLPP
#include <string.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Form.hpp>
#else
#include <string.h>
#include "curl/curl.h"
extern unsigned char bufff[40960];
extern int l;
int writer(unsigned char *data, size_t size, size_t nmemb, unsigned char *writerData);
#endif

const string PATH_LOGIN = "http://uwonders.ticp.net:8089/alarmupload/login";
const string PATH_ALARM = "http://uwonders.ticp.net:8089/alarmupload/alarm";
const string PATH_CONNT = "http://uwonders.ticp.net:8089/alarmupload/connt";
const string PATH_UPLOAD_FILE_INFO = "http://uwonders.ticp.net:8088/upload_file_info";
const string PATH_UPLOAD_BIG_FILE = "http://uwonders.ticp.net:8088/upload_big_file";
const string PATH_UPLOAD_FILE = "http://uwonders.ticp.net:8088/upload_file";

const int ERROR_CODES_LEN = 17;
const string _ERROR_CODES[ERROR_CODES_LEN * 2] = {
  "10000","执行成功",
  "10001","执行错误",
  "10002","服务器未知错误",
  "10003","Token超时",
  "10004","报文参数不正确",
  "10005","报文参数类型错误",

  "20001","发生错误",
  "20002","用户名或密码错误",

  "30001","上传的文件偏移和服务器保存的偏移不符",

  "30101","文件大小错误",
  "30102","分段大小错误",
  "30103","文件类型错误",
  "30104","文件具体类型错误",

  "30201","文件Hash（hashName）不存在",
  "30202","offset错误",
  "30203","fileData错误",
  "30204","服务器生成的文件Hash和客户端生成的文件Hash不一致",
};
#undef X_I
#undef X_S
#define X_I(i) (std::stoi(_ERROR_CODES[i*2]))
#define X_S(i) (_ERROR_CODES[i*2+1])
const int    N_ERROR[] = {X_I(0),X_I(1),X_I(2),X_I(3),X_I(4),X_I(5),X_I(6),X_I(7),X_I(8),X_I(9),X_I(10),X_I(11),X_I(12),X_I(13),X_I(14),X_I(15),X_I(16),};
const string S_ERROR[] = {X_S(0),X_S(1),X_S(2),X_S(3),X_S(4),X_S(5),X_S(6),X_S(7),X_S(8),X_S(9),X_S(10),X_S(11),X_S(12),X_S(13),X_S(14),X_S(15),X_S(16),};

const int ALARM_CODES_LEN = 6;
const string _ALARM_CODES[ALARM_CODES_LEN * 2] = {
  "3001","人脸识别",
  "3002","抽烟识别",
  "3003","打电话",
  "3004","未系安全带",
  "3005","疲劳检测",
  "3006","汽油泄漏",
};
#undef X_I
#undef X_S
#define X_I(i) std::stoi(_ALARM_CODES[2*i])
#define X_S(i) _ALARM_CODES[2*i+1]
const int    N_ALARM[] = {X_I(0),X_I(1),X_I(2),X_I(3),X_I(4),};
const string S_ALARM[] = {X_S(0),X_S(1),X_S(2),X_S(3),X_S(4),};
const int UW_FACE = 0;
const int UW_SMOKE = 1;
const int UW_PHONE = 2;
const int UW_UNBELT = 3;
const int UW_FATIGUE = 4;

class APIDEC Protocal {
 public:
  int verbose;
  Protocal();
  ~Protocal();

  int login(const string &userName,
	    const string &password,
	    int version,
	    Json &json);
  int alarm(const string &token,
	    int alarmType,
	    const string &alarmRecordDateTime,
	    const Json &arr,
	    int userId,
	    Json &json);
  int connt(const string &token,
	    int userId,
	    Json &json);

  int upload_file_info(const string &token,
		       int userId,
		       const string fileName,
		       const string fileHash,
		       int fileSize,
		       int sectionSize,
		       int fileType,
		       const string detailFileType,
		       int behaviourAlarmId,
		       int alarmRecordId,
		       Json &json);

  int upload_big_file(const string &token,
		      const string hashName,
		      long offset,
		      const string fileData,
		      Json &json);

  int upload_form_file(const string &fileName,
		       const string hashName,
		       Json &json);

  int upload_file(const string &token,
		  const int userId,
		  const char *buf,
		  const int buflen,
		  int behaviourAlarmId,
		  int alarmRecordId,
		  Json &json);
  string p_error(int code) const;

#ifdef CURLPP
  // curl part
  void curlpp_post(const string &server, const Json& req, std::stringstream &out);
  void curlpp_post(const string &server, const string &hashName, const string fileName, std::stringstream &out);
#else
  void curlpp_post(const string &server, const Json& req, std::stringstream &out);
  void curlpp_post(const string &server, const string &hashName, const string fileName, std::stringstream &out);
#endif
};

/*class Session {
 public:
  string mUsername;
  string mPassword;
  std::future<Json> fLogin;
  Json mBusInfo;
  mutex mtx;
  //
  string token, nickName;
  int userId;

  Protocal *mProtocal;
  vector<future<Json> > vf;

  Session(const string &plateNumber
	  ,const string &province
	  ,const string &city
	  ,const string &county
	  ,const string &busRoute
	  ,const string &driverName
	  ,const string &username="test"
	  ,const string &password="123456"
	  ) {
    mProtocal = new Protocal();
    mUsername = username;
    mPassword = password;
    mBusInfo = Json::object {
      {"plateNumber",plateNumber},
      {"province",province},
      {"city",city},
      {"county",county},
      {"busRoute",busRoute},
      {"driverName",driverName},
      {"eventTime",""},
      {"precisions",0},
    };
    
    token = nickName = "";
    fLogin = std::async(std::launch::async, login, this);
  }
  ~Session() {
    // sync all asyncs
    for(future<Json> &x : vf) {
      Json j = x.get();
    }
    delete mProtocal;
  }
  std::future<Json>& alarm_face(const char *buf, int buflen, double conf) {
    vf.push_back(std::async(std::launch::async, alarm, this, UW_FACE, buf, buflen, conf));
    return vf[vf.size()-1];
  }
  std::future<Json>& alarm_smoke(const char *buf, int buflen, double conf) {
    vf.push_back(std::async(std::launch::async, alarm, this, UW_SMOKE, buf, buflen, conf));
    return vf[vf.size()-1];
  }
  std::future<Json>& alarm_phone(const char *buf, int buflen, double conf) {
    vf.push_back(std::async(std::launch::async, alarm, this, UW_PHONE, buf, buflen, conf));
    return vf[vf.size()-1];
  }
  std::future<Json>& alarm_unbelt(const char *buf, int buflen, double conf) {
    vf.push_back(std::async(std::launch::async, alarm, this, UW_UNBELT, buf, buflen, conf));
    return vf[vf.size()-1];
  }
  std::future<Json>& alarm_fatigue(const char *buf, int buflen, double conf) {
    vf.push_back(std::async(std::launch::async, alarm, this, UW_FATIGUE, buf, buflen, conf));
    return vf[vf.size()-1];
  }
  static Json alarm(Session * ss, int type, const char *buf, int buflen, double precisions) {
    {
      std::lock_guard<std::mutex> lck (ss->mtx);
      if (ss->token == "") {
	Json json = ss->fLogin.get();
	if (N_ERROR[0] == json["resultCode"].int_value()) {
	  ss->token = json["token"].string_value();
	  ss->nickName = json["nickName"].string_value();
	  ss->userId = json["userId"].int_value();
	}
      }
    }

    time_t nowtime = time(NULL);
    tm *now = localtime(&nowtime);
    char bufTime[128];
    sprintf(bufTime, "%04d-%02d-%02d %02d:%02d:%02d",
	    now->tm_year+1900, now->tm_mon+1, now->tm_mday,
	    now->tm_hour, now->tm_min, now->tm_sec);

    string alarmRecordDateTime(bufTime);
    Json o = Json::object {
      {"plateNumber",ss->mBusInfo["plateNumber"]},
      {"province",ss->mBusInfo["province"]},
      {"city",ss->mBusInfo["city"]},
      {"county",ss->mBusInfo["county"]},
      {"busRoute",ss->mBusInfo["busRoute"]},
      {"driverName",ss->mBusInfo["driverName"]},
      {"eventTime",alarmRecordDateTime},
      {"precisions",precisions},
      {"hasImg",2},
    };
    Json json;
    ss->mProtocal->alarm(ss->token, type, alarmRecordDateTime, o, ss->userId, json);

    if (N_ERROR[0] != json["resultCode"].int_value()) {
      return json;
    }

    int alarmRecordId = json["alarmRecordId"].int_value();
    int behaviourAlarmId = json["contents"].int_value();

    json = Json();
    ss->mProtocal->upload_file(ss->token, ss->userId, buf, buflen, behaviourAlarmId, alarmRecordId, json);
    return json;
  }

  static Json login(Session *ss) {
    Json json;
    ss->mProtocal->login(ss->mUsername, ss->mPassword, 1, json);
    return json;
  }
};*/


#endif // ___PROTOCAL__H_
