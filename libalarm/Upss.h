#ifndef ___UPSS__H_
#define ___UPSS__H_
#include "config.h"
#include <string.h>
// 气味传感器底层处理协议
// Uart Portocal 4 Smell Sensor
class APIDEC Upss {
public:
	const static int LEN = 1024;
	const static int MAX_PACKAGE_LEN = 18;
	const static int MAX_HEADER_LEN = 2;
	const static int LOWMARK = MAX_PACKAGE_LEN + MAX_HEADER_LEN;
	typedef void(*RESPONSE_CB)(const unsigned char *data, int len);
	RESPONSE_CB response;
	unsigned char in[LEN];
	int write_ptr;
	int read_ptr;

	unsigned char out[LEN];

	Upss(RESPONSE_CB res = NULL);
	void process(const unsigned char *data, int len);
};
#endif