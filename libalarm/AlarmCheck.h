#ifndef ___ALARMCHECK__H_
#define ___ALARMCHECK__H_
#include "config.h"
class APIDEC AlarmCheck {
public:
	const int static N = 8;
	const int S_INIT = 0;
	const int S_READY = 1;
	const int S_ALARM = 2;

	const static int NORMAL_THRESHOLD = 1; // ���������������ֵ
	const static int NORMAL_MIN_LENGTH = 20; // �����ȶ�������Сֵ

	int values[N];
	int states[N];
	int acc[N];
	int alarmid[N];

	AlarmCheck();
	int check(int *v, int len);
};
#endif ___ALARMCHECK__H_