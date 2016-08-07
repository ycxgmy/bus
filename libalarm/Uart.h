#ifndef ___UART__H_
#define ___UART__H_
#include "config.h"
#include <windows.h>
#include <iostream>
#include <future>
#include <string>
using std::string;
// 串口数据读写发生在这个类里面

class Upss;
class APIDEC Uart {
public:
	string mPort;
	HANDLE  m_hComm;
	bool mQuit;
	std::future<void> fquit;
	unsigned char rbuf[1024];
	unsigned char wbuf[1024];
	Upss *up;
	Uart(int portNo = 1, Upss *u = NULL);
	~Uart();

	bool init();
	void run();
	void stop();
	void reset();
	static void read_process(Uart *u);
	static void ac(int *v, int len);
	static void response(const unsigned char *data, int len);

	std::future<void> mPnpWatcher;
	static void pnp_watcher(Uart *u);
};
#endif