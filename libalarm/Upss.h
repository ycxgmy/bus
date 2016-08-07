#ifndef ___UPSS__H_
#define ___UPSS__H_
#include "config.h"
#include <string.h>
// 气味传感器底层处理协议
// Uart Portocal 4 Smell Sensor
class Session;
class AlarmCheck;
class APIDEC Upss {
public:
	Session *ss;
	AlarmCheck *mAC;
	const static int LEN = 1024;
	const static int MAX_PACKAGE_LEN = 18;
	const static int MAX_HEADER_LEN = 2;
	const static int LOWMARK = MAX_PACKAGE_LEN + MAX_HEADER_LEN;
	unsigned char in[LEN];
	int write_ptr;
	int read_ptr;

	unsigned char out[LEN];

	Upss(Session *s = NULL);
	~Upss();
	void process(const unsigned char *data, int len);
	void response(const unsigned char *data, int len);
	void ac(int *v, int len);
};
#endif