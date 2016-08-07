#include "AlarmCheck.h"

#include <string.h>
#include <math.h>
#include <iostream>

AlarmCheck::AlarmCheck(int min_stable_len, int thre) {
	NORMAL_THRESHOLD = thre;
	NORMAL_MIN_LENGTH = min_stable_len;
	memset(values, 0, sizeof(values));
	memset(states, 0, sizeof(states));
	memset(acc, 0, sizeof(acc));
	memset(alarmid, 0, sizeof(alarmid));
}
int AlarmCheck::check(int *v, int len) {
	memset(alarmid, 0, sizeof(alarmid));
	int nalarm = 0;
	if (len > N)
		return nalarm;
	for (int i = 0; i < len; ++i) {
		if (abs(values[i] - v[i]) < NORMAL_THRESHOLD) {
			if (acc[i]++ > NORMAL_MIN_LENGTH) {
				if (states[i] != S_READY) {
					std::cout << "channel " << i << " ready" << std::endl;
				}
				states[i] = S_READY;
			}
		}
		else {
			values[i] = v[i];
			acc[i] = 0;

			if (states[i] == S_READY) {
				//alarm
				alarmid[i] = 1;
				nalarm++;
				states[i] = S_INIT;
			}
		}
	}
	return nalarm;
}