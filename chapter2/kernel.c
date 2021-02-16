#include <stdint.h> 
#include <xen.h>

#include "mytime.h"
#include <stddef.h>
#include <stdarg.h>

#if defined (__i686__)
#include "hypercall-x86_32.h"
#elif defined (__x86_64__)
#include "hypercall-x86_64.h"
#else
#error "Unsupported architecture"
#endif

#define BUFF_SIZE (128)

size_t strnlen(const char *s, size_t maxlen);
int snprintf(char *buf, size_t maxlen, const char *fmt, ...);

/* Some static space for the stack */
char stack[8192];

/* Main kernel entry point, called by trampoline */
void start_kernel(start_info_t * start_info)
{
	struct timeval tv;
	time_t sec_old = 0;
	char buff[BUFF_SIZE];

	HYPERVISOR_console_io(CONSOLEIO_write,12,"Hello World\n");
	while(1) {
		gettimeofday(&tv, NULL);
		if ( tv.tv_sec != sec_old ) {
			snprintf(buff, BUFF_SIZE, "%ld\n", tv.tv_sec);
			HYPERVISOR_console_io(CONSOLEIO_write, strnlen(buff, BUFF_SIZE), buff);
			sec_old = tv.tv_sec;
		}
	}
}
