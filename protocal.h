#ifndef ___PROTOCAL__H_
#define ___PROTOCAL__H_

#include "Base64.h"
#include "md5.h"
#include "json11/json11.hpp"
using namespace json11;

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <future>

//#include <openssl/md5.h>
//#define CURLPP
#ifdef CURLPP
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Form.hpp>
#else
#include <string.h>
#include "curl/curl.h"
unsigned char bufff[40960];
int l = 0;
int writer(unsigned char *data, size_t size, size_t nmemb, unsigned char *writerData)
{
int realsize = size * nmemb;
memcpy(bufff + l, data, realsize);
l += realsize;
return realsize;
}
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

const int ALARM_CODES_LEN = 5;
const string _ALARM_CODES[ALARM_CODES_LEN * 2] = {
  "3001","人脸识别",
  "3002","抽烟识别",
  "3003","打电话",
  "3004","未系安全带",
  "3005","疲劳检测",
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

class Protocal {
 public:
  Protocal() {
  }
  ~Protocal() {}

  int login(const string &userName,
	    const string &password,
	    int version,
	    Json &json)
  {
    Json body = Json::object {
      {"userName", userName},
      {"password", password},
      {"version", version},
    };
    Json req = Json::object {
      {"loginReq", body}
    };
    cout << "I: " << req.dump() << endl;


    std::stringstream out;
    curlpp_post(PATH_LOGIN, req, out);
    string err;
    Json res = Json::parse(out.str(), err);
    Json response = res["loginResp"];
    cout << "O: " << res.dump() << endl;
    int code = response["resultCode"].int_value();
    json = response;
    return code;
  }

  int alarm(const string &token,
	    int alarmType,
	    const string &alarmRecordDateTime,
	    const Json &arr,
	    int userId,
	    Json &json)
  {
    Json body = Json::object {
      {"token", token},
      {"userId", std::to_string(userId)},
      {"typeId", std::to_string(N_ALARM[alarmType])},
      //{"typeDesc", S_ALARM[alarmType]},
      //{"alarmRecordDateTime", alarmRecordDateTime},
      {"contents", arr},
    };
    Json req = Json::object {
      {"alarmReq", body}
    };
    cout << "I: " << req.dump() << endl;
    std::stringstream out;
    curlpp_post(PATH_ALARM, req, out);

    string err;
    Json res = Json::parse(out.str(), err);
    Json response = res["alarmResp"];
    cout << "O: " << res.dump() << endl;
    int code = response["resultCode"].int_value();
    json = response;
    return code;
  }

  int connt(const string &token,
	    int userId,
	    Json &json)
  {
    Json body = Json::object {
      {"token", token},
      {"userId", userId},
    };
    Json req = Json::object {
      {"conntReq", body}
    };
    cout << "I: " << req.dump() << endl;
    std::stringstream out;
    curlpp_post(PATH_CONNT, req, out);

    string err;
    Json res = Json::parse(out.str(), err);
    Json response = res["conntResp"];
    cout << "O: " << res.dump() << endl;
    int code = response["resultCode"].int_value();
    json = response;
    return code;
  }

  int upload_file_info(const string &token,
		       int userId,
		       const string fileName,
		       const string fileHash,
		       int fileSize,
		       int sectionSize,
		       int fileType,
		       const string detaiTFileType, // TODO typo
		       int behaviourAlarmId,
		       int alarmRecordId,
		       Json &json)
  {
    Json body = Json::object {
      {"token", token},
      {"userId", userId},
      {"fileName", fileName},
      {"fileHash", fileHash},
      {"fileSize", fileSize},
      {"sectionSize", sectionSize},
      {"fileType", fileType},
      {"detaiTFileType", detaiTFileType},
      {"behaviourAlarmId", behaviourAlarmId},
      {"alarmRecordId", alarmRecordId},
    };
    Json req = Json::object {
      {"uploadfileInfoReq", body}
    };
    cout << "I: " << req.dump() << endl;
    std::stringstream out;
    curlpp_post(PATH_UPLOAD_FILE_INFO, req, out);

    string err;
    Json res = Json::parse("{"+out.str(), err);
    Json response = res["uploadfileInfoReq"];
    cout << "O: " << res.dump() << endl;
    int code = response["resultCode"].int_value();
    json = response;
    return code;
  }

  int upload_big_file(const string &token,
		      const string hashName,
		      long offset,
		      const string fileData,
		      Json &json)
  {
    Json body = Json::object {
      {"token", token},
      {"hashName", hashName},
      {"offset", (int)offset}, // TODO
      {"fileData", fileData},
    };
    Json req = Json::object {
      {"uploadFileReq", body}
    };
    cout << "I: " << req.dump() << endl;
    std::stringstream out;
    curlpp_post(PATH_UPLOAD_BIG_FILE, req, out);

    string err;
    Json res = Json::parse("{"+out.str(), err);
    Json response = res["uploadFileReq"];
    cout << "O: " << res.dump() << endl;
    int code = response["resultCode"].int_value();
    json = response;
    return code;
  }

  int upload_form_file(const string &fileName,
		       const string hashName,
		       Json &json)
  {
    Json body = Json::object {
    };
    Json req = Json::object {
      {"uploadFileReq", body}
    };
    cout << "I: " << req.dump() << endl;
    std::stringstream out;
    curlpp_post(PATH_UPLOAD_FILE, hashName, fileName, out);
    /*try {
      curlpp::Cleanup clean;
      curlpp::Easy request;
      request.setOpt(new curlpp::options::Url(PATH_UPLOAD_FILE));
      {
	// Forms takes ownership of pointers!
	curlpp::Forms formParts;
	formParts.push_back(new curlpp::FormParts::File(hashName, fileName));
	request.setOpt(new curlpp::options::HttpPost(formParts));
      }

      request.setOpt(new curlpp::options::WriteStream(&out));
      request.setOpt(new curlpp::options::Verbose(false));
      request.perform();
    }
    catch ( curlpp::LogicError & e ) {
      std::cerr << e.what() << std::endl;
    }
    catch ( curlpp::RuntimeError & e ) {
      std::cerr << e.what() << std::endl;
      }*/
    string err;
    Json res = Json::parse("{"+out.str(), err);
    Json response = res["uploadFileReq"];
    cout << "O: " << res.dump() << endl;
    int code = response["resultCode"].int_value();
    json = response;
    return code;
  }

  int upload_file(const string &token,
		  const int userId,
		  const string &fileName,
		  int behaviourAlarmId,
		  int alarmRecordId,
		  Json &json)
  {
    string fileHash = "";
    int fileSize = 0;
    int sectionSize = 0;
    int fileType = 1;
    string detaiTFileType = "jpg";
    if (fileName.length() > 3) {
      detaiTFileType = fileName.substr(fileName.length() - 3);
    } else {
      detaiTFileType = "err";
    }
    string fileData = "";

    ifstream in(fileName, ios::binary);
    if (!in) {
      return -1;
    }

    ostringstream os;
    os << in.rdbuf();
    fileData = os.str();
    Base64::encode(fileData);

    in.seekg(0,ios_base::end);
    fileSize = in.tellg(); //
    sectionSize = fileSize; //

    in.seekg(0,ios_base::beg);
    MD5 md5;
    md5.reset();
    md5.update(in);
    md5.digest();
    fileHash = md5.toString(); //
    cout << fileSize << " " << fileHash << endl;

    in.seekg(0,ios_base::beg);

    in.close();

    // upload_file_info
    json = Json();
    int code = upload_file_info(token, userId, fileName, fileHash, fileSize, sectionSize, fileType, detaiTFileType, behaviourAlarmId, alarmRecordId, json);
    if (code != N_ERROR[0]) {
      return -2;
    }

    // try to upload by method 1
    /*json = Json();
    code = upload_form_file(fileName, fileHash, json);
    if (code == N_ERROR[0]) {
      return code;
      }*/

    // try to upload by method 2
    json = Json();
    long offset = 0;
    return upload_big_file(token, fileHash, offset, fileData, json);
  }
  string p_error(int code) const {
    for (int i = 0; i < sizeof(N_ERROR)/sizeof(N_ERROR[0]); ++i) {
      if (code == N_ERROR[i]) {
	return S_ERROR[i];
      }
    }
    return "未定义错误";
  }

#ifdef CURLPP
  // curl part
  void curlpp_post(const string &server, const Json& req, std::stringstream &out) {
    try {
      curlpp::Cleanup clean;
      curlpp::Easy request;
      request.setOpt(new curlpp::options::Url(server));

      std::list<std::string> headers;
      headers.push_back("Content-Type: application/json");
      request.setOpt(new curlpp::options::HttpHeader(headers));

      std::string test = req.dump();
      request.setOpt(new curlpp::options::PostFields(test));
      request.setOpt(new curlpp::options::PostFieldSize(test.length()));

      request.setOpt(new curlpp::options::WriteStream(&out));
      request.setOpt(new curlpp::options::Verbose(false));
      request.perform();
    }
    catch ( curlpp::LogicError & e ) {
      std::cerr << e.what() << std::endl;
    }
    catch ( curlpp::RuntimeError & e ) {
      std::cerr << e.what() << std::endl;
    }
  }
  void curlpp_post(const string &server, const string &hashName, const string fileName, /*const Json& req,*/ std::stringstream &out) {
    try {
      curlpp::Cleanup clean;
      curlpp::Easy request;
      request.setOpt(new curlpp::options::Url(server));

      // Forms takes ownership of pointers!
      curlpp::Forms formParts;
      formParts.push_back(new curlpp::FormParts::File(hashName, fileName));
      request.setOpt(new curlpp::options::HttpPost(formParts));

      request.setOpt(new curlpp::options::WriteStream(&out));
      request.setOpt(new curlpp::options::Verbose(false));
      request.perform();
    }
    catch ( curlpp::LogicError & e ) {
      std::cerr << e.what() << std::endl;
    }
    catch ( curlpp::RuntimeError & e ) {
      std::cerr << e.what() << std::endl;
    }
  }
#else
  void curlpp_post(const string &server, const Json& req, std::stringstream &out) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    l = 0;
    memset(bufff, 0, sizeof(bufff));

    curl = curl_easy_init();
    if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, server.c_str());

      curl_slist *plist = curl_slist_append(NULL, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist); 
      string str = req.dump();
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

      res = curl_easy_perform(curl);
      if (res != CURLE_OK)
	fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

      out.write((char*)bufff, l);
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
  }
  void curlpp_post(const string &server, const string &hashName, const string fileName, /*const Json& req,*/ std::stringstream &out) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    l = 0;
    memset(bufff, 0, sizeof(bufff));

    curl = curl_easy_init();
    if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, server.c_str());

      curl_slist *plist = curl_slist_append(NULL, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist); 
      struct curl_httppost *post=NULL;
      struct curl_httppost *last=NULL;
      curl_formadd(&post, &last, CURLFORM_COPYNAME, hashName.c_str(),
		   CURLFORM_FILE, fileName.c_str(), CURLFORM_END);
      curl_formadd(&post, &last,
		   CURLFORM_COPYNAME, "submit",
		   CURLFORM_COPYCONTENTS, "OK",
		   CURLFORM_END);

      curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
      //string str = req.dump();
      //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

      res = curl_easy_perform(curl);
      if (res != CURLE_OK)
	fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

      out.write((char*)bufff, l);
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
  }
#endif
};


class Session {
 public:
  string mUsername;
  string mPassword;
  std::future<Json> fLogin;
  Json mBusInfo;

  //
  string token, nickName;
  int userId;

  Protocal *mProtocal;

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
    delete mProtocal;
  }
  void alarm_face(const string &imagefile, double conf) {
    std::async(std::launch::async, alarm, this, FACE, imagefile, conf);
  }
  void alarm_smoke(const string &imagefile, double conf) {
    std::async(std::launch::async, alarm, this, UW_SMOKE, imagefile, conf);
  }
  void alarm_phone(const string &imagefile, double conf) {
    std::async(std::launch::async, alarm, this, UW_PHONE, imagefile, conf);
  }
  void alarm_unbelt(const string &imagefile, double conf) {
    std::async(std::launch::async, alarm, this, UW_UNBELT, imagefile, conf);
  }
  void alarm_fatigue(const string &imagefile, double conf) {
    std::async(std::launch::async, alarm, this, UW_FATIGUE, imagefile, conf);
  }
  static void alarm(Session * ss, int type, string imagefile, double precisions) {
    if (ss->token == "") {
      Json json = ss->fLogin.get();
      if (N_ERROR[0] == json["resultCode"].int_value()) {
	//int resultCode = json["resultCode"].int_value();
	ss->token = json["token"].string_value();
	//string tokenExpires = json["tokenExpires"].string_value();
	//string userName = json["userName"].string_value();
	ss->nickName = json["nickName"].string_value();
	ss->userId = json["userId"].int_value();
      }
    }

    string alarmRecordDateTime = "2016-5-19 14:13:59";
    Json o = Json::object {
      {"plateNumber",ss->mBusInfo["plateNumber"]},
      {"province",ss->mBusInfo["province"]},
      {"city",ss->mBusInfo["city"]},
      {"county",ss->mBusInfo["county"]},
      {"busRoute",ss->mBusInfo["busRoute"]},
      {"driverName",ss->mBusInfo["driverName"]},
      {"eventTime","2016-5-19 14:13:59"},
      {"precisions",precisions},
    };
    Json json;
    ss->mProtocal->alarm(ss->token, type, alarmRecordDateTime, o, ss->userId, json);
    if (N_ERROR[0] != json["resultCode"].int_value()) {
      return;
    }

    int alarmRecordId = json["alarmRecordId"].int_value();
    int behaviourAlarmId = json["contents"].int_value();

    json = Json();
    if (N_ERROR[0] != ss->mProtocal->upload_file(ss->token, ss->userId, imagefile, behaviourAlarmId, alarmRecordId, json)) {
      return;
    }
    
  }
  static Json login(Session *ss) {
    Json json;
    ss->mProtocal->login(ss->mUsername, ss->mPassword, 1, json);
    return json;
  }
};


#endif // ___PROTOCAL__H_
