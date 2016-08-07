#include "session.h"
#include "protocal.h"

Session::Session(const string &plateNumber
	, const string &province
	, const string &city
	, const string &county
	, const string &busRoute
	, const string &driverName
	, const string &username
	, const string &password
	, const int verbose
	) {
	mProtocal = new Protocal(verbose);
	mUsername = username;
	mPassword = password;
	Json json = Json::object{
		{ "plateNumber",plateNumber },
		{ "province",province },
		{ "city",city },
		{ "county",county },
		{ "busRoute",busRoute },
		{ "driverName",driverName },
		{ "eventTime","" },
		{ "precisions",0 },
	};
	mBusInfo = json.dump();

	token = nickName = "";
	fLogin = std::async(std::launch::async, login, this);
}
Session::~Session() {
	// sync all asyncs
	for (future<string> &x : vf) {
		string j = x.get();
	}
	delete mProtocal;
}
std::future<string>& Session::alarm_face(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_FACE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<string>& Session::alarm_smoke(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_SMOKE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<string>& Session::alarm_phone(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_PHONE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<string>& Session::alarm_unbelt(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_UNBELT, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<string>& Session::alarm_fatigue(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_FATIGUE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<string>& Session::alarm_gas(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_GAS, buf, buflen, conf));
	return vf[vf.size() - 1];
}
string Session::alarm(Session * ss, int type, const char *buf, int buflen, double precisions) {
	{
		std::lock_guard<std::mutex> lck(ss->mtx);
		if (ss->token == "") {
			string err;
			Json json = Json::parse(ss->fLogin.get(), err);
			if (N_ERROR[0] == json["resultCode"].int_value()) {
				ss->token = json["token"].string_value();
				ss->nickName = json["nickName"].string_value();
				ss->userId = json["userId"].int_value();
			}
			else {
				ss->fLogin = std::async(std::launch::async, login, ss);
				if (ss->mProtocal->verbose != 0) {
					cerr << "login failed, retrying ..." << endl;
				}
				return json.dump();
			}
		}
	}

	time_t nowtime = time(NULL);
	tm *now = localtime(&nowtime);
	char bufTime[128];
	sprintf(bufTime, "%04d-%02d-%02d %02d:%02d:%02d",
		now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec);

	int hasImg = 2;
	if (type == UW_GAS) {
		hasImg = 0;
	}
	string alarmRecordDateTime(bufTime);
	string err;
	Json busInfo = Json::parse(ss->mBusInfo, err);
	Json o = Json::object{
		{ "plateNumber",busInfo["plateNumber"] },
		{ "province",busInfo["province"] },
		{ "city",busInfo["city"] },
		{ "county",busInfo["county"] },
		{ "busRoute",busInfo["busRoute"] },
		{ "driverName",busInfo["driverName"] },
		{ "eventTime",alarmRecordDateTime },
		{ "precisions",precisions },
		{ "hasImg", hasImg },
	};
	Json json;
	ss->mProtocal->alarm(ss->token, type, alarmRecordDateTime, o, ss->userId, json);

	if (N_ERROR[0] != json["resultCode"].int_value()) {
		return json.dump();
	}
	if (hasImg == 0) {
		return json.dump();
	}

	int alarmRecordId = json["alarmRecordId"].int_value();
	int behaviourAlarmId = json["contents"].int_value();

	json = Json();
	ss->mProtocal->upload_file(ss->token, ss->userId,/* imagefile,*/ buf, buflen, behaviourAlarmId, alarmRecordId, json);
	return json.dump();
}

string Session::login(Session *ss) {
	Json json;
	ss->mProtocal->login(ss->mUsername, ss->mPassword, 1, json);
	return json.dump();
}