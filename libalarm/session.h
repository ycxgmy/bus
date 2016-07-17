#pragma once
#include <string>
#include <vector>
#include <future>
#include "config.h"
class Protocal;
using std::string;
using std::mutex;
using std::vector;
using std::future;

class APIDEC Session {
public:
	string mUsername;
	string mPassword;
	future<string> fLogin;
	string mBusInfo;
	mutex mtx;

	string token, nickName;
	int userId;

	Protocal *mProtocal;
	vector<future<string> > vf;

	Session(const string &plateNumber
		, const string &province
		, const string &city
		, const string &county
		, const string &busRoute
		, const string &driverName
		, const string &username = "test"
		, const string &password = "123456"
		, const int verbose = 1
		);
	~Session();
	std::future<string>& alarm_face(const char *buf, int buflen, double conf);
	std::future<string>& alarm_smoke(const char *buf, int buflen, double conf);
	std::future<string>& alarm_phone(const char *buf, int buflen, double conf);
	std::future<string>& alarm_unbelt(const char *buf, int buflen, double conf);
	std::future<string>& alarm_fatigue(const char *buf, int buflen, double conf);
	static string alarm(Session * ss, int type, const char *buf, int buflen, double precisions);
	static string login(Session *ss);
};
