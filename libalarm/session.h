#pragma once
#include <string>
#include <vector>
#include <future>
#include "json11/json11.hpp"
class Protocal;
using std::string;
using std::mutex;
using std::vector;
using std::future;
using json11::Json;

class APIDEC Session {
public:
	string mUsername;
	string mPassword;
	future<string> fLogin;
	Json mBusInfo;
	mutex mtx;

	string token, nickName;
	int userId;

	Protocal *mProtocal;
	vector<future<Json> > vf;

	Session(const string &plateNumber
		, const string &province
		, const string &city
		, const string &county
		, const string &busRoute
		, const string &driverName
		, const string &username = "test"
		, const string &password = "123456"
		);
	~Session();
	std::future<Json>& alarm_face(const char *buf, int buflen, double conf);
	std::future<Json>& alarm_smoke(const char *buf, int buflen, double conf);
	std::future<Json>& alarm_phone(const char *buf, int buflen, double conf);
	std::future<Json>& alarm_unbelt(const char *buf, int buflen, double conf);
	std::future<Json>& alarm_fatigue(const char *buf, int buflen, double conf);
	static Json alarm(Session * ss, int type, const char *buf, int buflen, double precisions);
	static string login(Session *ss);
};
