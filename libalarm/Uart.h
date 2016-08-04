#ifndef ___UART__H_
#define ___UART__H_
#include "config.h"
#include <windows.h>
#include <iostream>
#include <future>
// 串口数据读写发生在这个类里面

class Upss;
class APIDEC Uart {
public:
	HANDLE  m_hComm;
	bool mQuit;
	std::future<void> fquit;
	unsigned char rbuf[1024];
	unsigned char wbuf[1024];
	Upss *up;
	Uart(int portNo, Upss *u);
	~Uart();
	void run();
	void stop();
	void reset();
	bool read_byte(unsigned char &cRecved);
	int avalable();
	static void read_process(Uart *u);
	static void ac(int *v, int len);
	static void response(const unsigned char *data, int len);
};
#endif