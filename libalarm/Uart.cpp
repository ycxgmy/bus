#include "Uart.h"
#include <iostream>
#include <string>
using std::cerr;
using std::cout;
using std::endl;
#include <future>
#include "AlarmCheck.h"
#include "Upss.h"

Uart::Uart(int portNo, Upss *u) {
	mPort = "COM" + std::to_string(portNo);
	up = u;

	mQuit = false;
	init();

	// start reader
	fquit = std::async(std::launch::async, read_process, this);
	// start watcher
	mPnpWatcher = std::async(std::launch::async, pnp_watcher, this);
}
Uart::~Uart() {
	if (m_hComm != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
	
	mQuit = true;
	fquit.get();
	mPnpWatcher.get();
}

bool Uart::init() {
	reset();

	m_hComm = CreateFile(mPort.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	if (m_hComm != INVALID_HANDLE_VALUE) {
		SetupComm(m_hComm, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024

		DCB  dcb;
		GetCommState(m_hComm, &dcb);
		dcb.BaudRate = CBR_9600; //波特率为9600
		dcb.ByteSize = 8; //每个字节有8位
		dcb.Parity = NOPARITY; //无奇偶校验位
		dcb.StopBits = ONE5STOPBITS; //一个停止位
		SetCommState(m_hComm, &dcb);
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	}
	else {
		cerr << "Failed to open " << mPort << endl;
	}
	return true;
}
void Uart::run() {
}
void Uart::stop() {
}
void Uart::reset() {
}

void Uart::read_process(Uart *u) {
	std::cout << "Thread: read_process running" << endl;
	while (!u->mQuit) {
		if (u->m_hComm == INVALID_HANDLE_VALUE) {
			Sleep(100);
			continue;
		}

		DWORD wCount = 0;//读取的字节数
		BOOL bReadStat;
		bReadStat = ReadFile(u->m_hComm, u->rbuf, 4, &wCount, NULL);
		if (!bReadStat) {
			cerr << "read error" << endl;
			CloseHandle(u->m_hComm);
			u->m_hComm = INVALID_HANDLE_VALUE;
			continue;
		}
		else if (wCount == 0) {
			Sleep(50);
		} else { 
			u->up->process(u->rbuf, wCount);
		}
	}
}

void Uart::ac(int *v, int len) {
	static AlarmCheck ac;
	if (ac.check(v, 8) > 0) {
		std::cout << "Alarm id:";
		for (int i = 0; i < ac.N; ++i) {
			if (ac.alarmid[i] > 0)
				std::cout << i << " ";
		}
		std::cout << std::endl;
	}
}
void Uart::response(const unsigned char *data, int len) {
	int channels[8];
	for (int i = 0; i < 8; ++i) {
		channels[i] = data[2 * i + 0] | data[2 * i + 0 + 1] << 8;
		std::cout << channels[i] << "\t";
	}
	std::cout << std::endl;
	ac(channels, 8);
}

void Uart::pnp_watcher(Uart *u) {
	std::cout << "Thread: pnp_wacher is running" << endl;
	while (!u->mQuit) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (u->m_hComm != INVALID_HANDLE_VALUE) {
			continue;
		}
		// retry com port
		u->init();
	}
}