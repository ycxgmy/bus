#ifndef ___UPSS__H_
#define ___UPSS__H_
#include <string.h>
// 气味传感器底层处理协议
// Uart Portocal 4 Smell Sensor
class upss {
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

	upss(RESPONSE_CB res = NULL) {
		response = res;
		write_ptr = 0;
		read_ptr = 0;
	}
	void process(const unsigned char *data, int len) {
		// TODO, if len is larger than valid buffer
		memcpy(in + write_ptr, data, len);
		write_ptr += len;

		// process
		while (true) {
			int remaining = write_ptr - read_ptr;
			if (remaining < LOWMARK) {
				break;
			}

			if (in[read_ptr] == 0x55 && in[read_ptr + 1] == 0xaa
				&& in[read_ptr + 18] == 0x55 && in[read_ptr + 1 + 18] == 0xaa
				) {
				if (response) {
					response(in + read_ptr + 2, 18 - 2);
				}
				read_ptr += 18;
			}
			else {
				read_ptr++;
			}

			remaining = write_ptr - read_ptr;
			if (remaining < LOWMARK) {
				char tmp[LOWMARK];
				memcpy(tmp, in + read_ptr, remaining);
				memcpy(in, tmp, remaining);
				write_ptr = remaining;
				read_ptr = 0;
				break;
			}
		}
	}
};
#endif