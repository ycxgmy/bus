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
	) {
	mProtocal = new Protocal();
	mUsername = username;
	mPassword = password;
	mBusInfo = Json::object{
		{ "plateNumber",plateNumber },
		{ "province",province },
		{ "city",city },
		{ "county",county },
		{ "busRoute",busRoute },
		{ "driverName",driverName },
		{ "eventTime","" },
		{ "precisions",0 },
	};

	token = nickName = "";
	fLogin = std::async(std::launch::async, login, this);
}
Session::~Session() {
	// sync all asyncs
	for (future<Json> &x : vf) {
		Json j = x.get();
	}
	delete mProtocal;
}
std::future<Json>& Session::alarm_face(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_FACE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<Json>& Session::alarm_smoke(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_SMOKE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<Json>& Session::alarm_phone(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_PHONE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<Json>& Session::alarm_unbelt(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_UNBELT, buf, buflen, conf));
	return vf[vf.size() - 1];
}
std::future<Json>& Session::alarm_fatigue(const char *buf, int buflen, double conf) {
	vf.push_back(std::async(std::launch::async, alarm, this, UW_FATIGUE, buf, buflen, conf));
	return vf[vf.size() - 1];
}
Json Session::alarm(Session * ss, int type, const char *buf, int buflen, double precisions) {
	{
		std::lock_guard<std::mutex> lck(ss->mtx);
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
		now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec);

	string alarmRecordDateTime(bufTime);
	Json o = Json::object{
		{ "plateNumber",ss->mBusInfo["plateNumber"] },
		{ "province",ss->mBusInfo["province"] },
		{ "city",ss->mBusInfo["city"] },
		{ "county",ss->mBusInfo["county"] },
		{ "busRoute",ss->mBusInfo["busRoute"] },
		{ "driverName",ss->mBusInfo["driverName"] },
		{ "eventTime",alarmRecordDateTime },
		{ "precisions",precisions },
		{ "hasImg",2 },
	};
	Json json;
	ss->mProtocal->alarm(ss->token, type, alarmRecordDateTime, o, ss->userId, json);

	if (N_ERROR[0] != json["resultCode"].int_value()) {
		return json;
	}

	int alarmRecordId = json["alarmRecordId"].int_value();
	int behaviourAlarmId = json["contents"].int_value();

	json = Json();
	ss->mProtocal->upload_file(ss->token, ss->userId,/* imagefile,*/ buf, buflen, behaviourAlarmId, alarmRecordId, json);
	return json;
}

Json Session::login(Session *ss) {
	Json json;
	ss->mProtocal->login(ss->mUsername, ss->mPassword, 1, json);
	return json;
}