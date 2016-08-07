#include "Uart.h"
#include <iostream>
using std::cerr;
using std::endl;
#include <future>
#include "AlarmCheck.h"
#include "Upss.h"

Uart::Uart(int portNo, Upss *u) {
	up = u;
	mQuit = true;
	/** 把串口的编号转换为设备名 */
	char szPort[50];
	sprintf_s(szPort, "COM%d", portNo);

	/** 打开指定的串口 */
	m_hComm = CreateFileA(szPort,		                /** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE,  /** 访问模式,可同时读写 */
		0,                             /** 共享模式,0表示不共享 */
		NULL,							/** 安全性设置,一般使用NULL */
		OPEN_EXISTING,					/** 该参数表示设备必须存在,否则创建失败 */
		0,
		0);

	if (!m_hComm) {
		cerr << "Failed to open " << szPort << endl;
		return;
	}
	DCB  dcb;
	GetCommState(m_hComm, &dcb);
	//cout << dcb.BaudRate << " " << dcb.ByteSize << endl;
	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.StopBits = 1;
	SetCommState(m_hComm, &dcb);
}
Uart::~Uart() {
	fquit.get();
}
void Uart::run() {
	if (m_hComm != NULL) {
		mQuit = false;
	}
	fquit = std::async(std::launch::async, read_process, this);
}
void Uart::stop() {
	mQuit = true;
}
void Uart::reset() {

}
bool Uart::read_byte(unsigned char &cRecved)
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** 临界区保护 */
	//EnterCriticalSection(&m_csCommunicationSync);

	/** 从缓冲区读取一个字节的数据 */
	bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
	if ((!bResult))
	{
		/** 获取错误码,可以根据该错误码查出错误原因 */
		DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		//LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
	//LeaveCriticalSection(&m_csCommunicationSync);

	return (BytesRead == 1);

}
int Uart::avalable() {
	DWORD dwError = 0;	/** 错误码 */
	COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
	}
	return BytesInQue;
}

void Uart::read_process(Uart *u) {
	while (!u->mQuit) {
		int nv = u->avalable();
		if (nv > 0) {
			int len = std::min(nv, (int)sizeof(rbuf));
			int i = 0;
			for (; i < len; ++i) {
				unsigned char cRecved = 0x00;
				if (u->read_byte(cRecved) == true)
				{
					u->rbuf[i] = cRecved;
				}
				else {
					break;
				}
			}
			u->up->process(u->rbuf, i);

		}
		else {
			Sleep(50);
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