#include "Upss.h"

Upss::Upss(RESPONSE_CB res) {
	response = res;
	write_ptr = 0;
	read_ptr = 0;
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