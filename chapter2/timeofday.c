#include "mytime.h"
#include <stdint.h>
#include <xen.h>


#define NANOSEC(tsc)	((tsc<<shared_info->vcpu_info[0].time.tsc_shift)\
							* shared_info->vcpu_info[0].time.tsc_to_system_mul)
#define RDTSC(x)		__asm volatile ( "RDTSC":"=A" (tsc) )

//extern char shared_info[4096];
extern shared_info_t* shared_info;


int gettimeofday(struct timeval *tp, struct timezone *tzp) {
	uint64_t tsc;
	/* get the time values from the shared info page */
	uint32_t version, wc_version;
	uint32_t seconds, nanoseconds, system_time;
	uint64_t old_tsc;

	if (!tp) {
		return -1;
	}

	/* loop until we read all required values from the same update */
	do {
		/* spin if the time value is being updated */
		do {
			wc_version = shared_info->wc_version;
			version = shared_info->vcpu_info[0].time.version;
		} while (((version & 1) == 1) || ((wc_version & 1) == 1));
		
		/* read the values */
		seconds = shared_info->wc_sec;
		nanoseconds = shared_info->wc_nsec;
		system_time = shared_info->vcpu_info[0].time.system_time;
		old_tsc = shared_info->vcpu_info[0].time.tsc_timestamp;
	} while ((version != shared_info->vcpu_info[0].time.version) || 
			 (wc_version != shared_info->wc_version));

	/* get the current TSC value */
	RDTSC(tsc);

	/* get the number of elapsed cycles */
	tsc -= old_tsc;

	/* Update the system time */
	system_time += NANOSEC(tsc);
	nanoseconds += system_time;
	seconds += nanoseconds / 1000000000;
	nanoseconds = nanoseconds % 1000000000;

	/* return seconds and milliseconds values */
	tp->tv_sec = seconds;
	tp->tv_usec = nanoseconds * 1000;

	return 0;
}
