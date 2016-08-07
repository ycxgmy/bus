#include "Upss.h"
#include <iostream>
#include <string>
using std::cerr;
using std::cout;
using std::endl;
#include "AlarmCheck.h"
#include "session.h"

Upss::Upss(Session *s) {
	ss = s;
	mAC = new AlarmCheck();
	write_ptr = 0;
	read_ptr = 0;
}
Upss::~Upss() {
	if (mAC) {
		delete mAC;
		mAC = NULL;
	}
}
void Upss::process(const unsigned char *data, int len) {
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
			response(in + read_ptr + 2, 18 - 2);
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

void Upss::response(const unsigned char *data, int len) {
	int channels[8];
	for (int i = 0; i < 8; ++i) {
		channels[i] = data[2 * i + 0] | data[2 * i + 0 + 1] << 8;
		//std::cout << channels[i] << "\t";
	}
	//std::cout << std::endl;
	ac(channels, 8);
}

void Upss::ac(int *v, int len) {
	AlarmCheck &ac = *mAC;
	if (ac.check(v, 8) > 0) {
		std::cout << "Alarm id:";
		for (int i = 0; i < ac.N; ++i) {
			if (ac.alarmid[i] > 0) {
				std::cout << i << " ";
				if (ss) {
					ss->alarm_gas(NULL, 0, 0.9);
				}
			}
		}
		std::cout << std::endl;
	}
}