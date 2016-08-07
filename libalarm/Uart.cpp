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
	/** �Ѵ��ڵı��ת��Ϊ�豸�� */
	char szPort[50];
	sprintf_s(szPort, "COM%d", portNo);

	/** ��ָ���Ĵ��� */
	m_hComm = CreateFileA(szPort,		                /** �豸��,COM1,COM2�� */
		GENERIC_READ | GENERIC_WRITE,  /** ����ģʽ,��ͬʱ��д */
		0,                             /** ����ģʽ,0��ʾ������ */
		NULL,							/** ��ȫ������,һ��ʹ��NULL */
		OPEN_EXISTING,					/** �ò�����ʾ�豸�������,���򴴽�ʧ�� */
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

	/** �ٽ������� */
	//EnterCriticalSection(&m_csCommunicationSync);

	/** �ӻ�������ȡһ���ֽڵ����� */
	bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
	if ((!bResult))
	{
		/** ��ȡ������,���Ը��ݸô�����������ԭ�� */
		DWORD dwError = GetLastError();

		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		//LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */
	//LeaveCriticalSection(&m_csCommunicationSync);

	return (BytesRead == 1);

}
int Uart::avalable() {
	DWORD dwError = 0;	/** ������ */
	COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */
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