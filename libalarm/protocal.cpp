#include "protocal.h"
#ifdef CURLPP
#else
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

Protocal::Protocal() {
  verbose = 1;
#ifndef CURLPP
  curl_global_init(CURL_GLOBAL_ALL);
#endif
}

Protocal::~Protocal() {
#ifndef CURLPP
  curl_global_cleanup();
#endif
}

int Protocal::login(const string &userName,
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
  if (verbose > 0)
    cout << "I: " << req.dump() << endl;


  std::stringstream out;
  curlpp_post(PATH_LOGIN, req, out);
  string err;
  Json res = Json::parse(out.str(), err);
  Json response = res["loginResp"];
  if (verbose > 0)
    cout << "O: " << res.dump() << endl;
  int code = response["resultCode"].int_value();
  json = response;
  return code;
}


int Protocal::alarm(const string &token,
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
    {"contents", arr},
  };
  Json req = Json::object {
    {"alarmReq", body}
  };
  if (verbose > 0)
    cout << "I: " << req.dump() << endl;
  std::stringstream out;
  curlpp_post(PATH_ALARM, req, out);

  string err;
  Json res = Json::parse(out.str(), err);
  Json response = res["alarmResp"];
  if (verbose > 0)
    cout << "O: " << res.dump() << endl;
  int code = response["resultCode"].int_value();
  json = response;
  return code;
}

int Protocal::connt(const string &token,
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
  if (verbose > 0)
    cout << "I: " << req.dump() << endl;
  std::stringstream out;
  curlpp_post(PATH_CONNT, req, out);

  string err;
  Json res = Json::parse(out.str(), err);
  Json response = res["conntResp"];
  if (verbose > 0)
    cout << "O: " << res.dump() << endl;
  int code = response["resultCode"].int_value();
  json = response;
  return code;
}


int Protocal::upload_file_info(const string &token,
			       int userId,
			       const string fileName,
			       const string fileHash,
			       int fileSize,
			       int sectionSize,
			       int fileType,
			       const string detailFileType, // TODO typo
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
    {"detailFileType", detailFileType},
    {"behaviourAlarmId", behaviourAlarmId},
    {"alarmRecordId", alarmRecordId},
  };
  Json req = Json::object {
    {"uploadfileInfoReq", body}
  };
  if (verbose > 0)
    cout << "I: " << req.dump() << endl;
  std::stringstream out;
  curlpp_post(PATH_UPLOAD_FILE_INFO, req, out);

  string err;
  Json res = Json::parse("{"+out.str(), err);
  Json response = res["uploadfileInfoReq"];
  if (verbose > 0)
    cout << "O: " << res.dump() << endl;
  int code = response["resultCode"].int_value();
  json = response;
  return code;
}


int Protocal::upload_big_file(const string &token,
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
  if (verbose > 0)
    cout << "I: " << req.dump() << endl;
  std::stringstream out;
  curlpp_post(PATH_UPLOAD_BIG_FILE, req, out);

  string err;
  Json res = Json::parse("{"+out.str(), err);
  Json response = res["uploadFileReq"];
  if (verbose > 0)
    cout << "O: " << res.dump() << endl;
  int code = response["resultCode"].int_value();
  json = response;
  return code;
}


int Protocal::upload_form_file(const string &fileName,
			       const string hashName,
			       Json &json)
{
  Json body = Json::object {
  };
  Json req = Json::object {
    {"uploadFileReq", body}
  };
  if (verbose > 0)
    cout << "I: " << req.dump() << endl;
  std::stringstream out;
  curlpp_post(PATH_UPLOAD_FILE, hashName, fileName, out);
  string err;
  Json res = Json::parse("{"+out.str(), err);
  Json response = res["uploadFileReq"];
  if (verbose > 0)
    cout << "O: " << res.dump() << endl;
  int code = response["resultCode"].int_value();
  json = response;
  return code;
}


int Protocal::upload_file(const string &token,
			  const int userId,
			  const char *buf,
			  const int buflen,
			  int behaviourAlarmId,
			  int alarmRecordId,
			  Json &json)
{
  int fileSize = buflen;
  int sectionSize = fileSize;
  int fileType = 1;
  string detailFileType = "jpg";

  // new codes
  string base64code;
  Base64::encode(buf, buflen, base64code);
  string fileData = base64code;

  char md5sum[MD5_STR_LEN + 1];
  memset(md5sum, 0, sizeof(md5sum));
  Compute_string_md5((unsigned char*)buf, buflen, md5sum);
  string fileHash = string(md5sum);


  // upload_file_info
  json = Json();
  int code = upload_file_info(token, userId, "cache.jpg", fileHash, fileSize, sectionSize, fileType, detailFileType, behaviourAlarmId, alarmRecordId, json);
  if (code != N_ERROR[0]) {
    delete buf;
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
  int codee = upload_big_file(token, fileHash, offset, fileData, json);
  delete buf;
  return codee;
}
string Protocal::p_error(int code) const {
  for (int i = 0; i < sizeof(N_ERROR)/sizeof(N_ERROR[0]); ++i) {
    if (code == N_ERROR[i]) {
      return S_ERROR[i];
    }
  }
  return "未定义错误";
}

#ifdef CURLPP
// curl part
void Protocal::curlpp_post(const string &server, const Json& req, std::stringstream &out) {
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
void Protocal::curlpp_post(const string &server, const string &hashName, const string fileName, /*const Json& req,*/ std::stringstream &out) {
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
void Protocal::curlpp_post(const string &server, const Json& req, std::stringstream &out) {
  CURL *curl;
  CURLcode res;
  //curl_global_init(CURL_GLOBAL_ALL);
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
  //curl_global_cleanup();
}
void Protocal::curlpp_post(const string &server, const string &hashName, const string fileName, /*const Json& req,*/ std::stringstream &out) {
  CURL *curl;
  CURLcode res;
  //curl_global_init(CURL_GLOBAL_ALL);
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
  //curl_global_cleanup();
}
#endif


