#include <stdint.h> 
#include <xen.h>

#include "mytime.h"
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#if defined (__i686__)
#include "hypercall-x86_32.h"
#elif defined (__x86_64__)
#include "hypercall-x86_64.h"
#else
#error "Unsupported architecture"
#endif

#include "hypercall_dbg-x86_64.h"

#define BUFF_SIZE (128)
#define SIBUFF_SZ (512)

size_t strnlen(const char *s, size_t maxlen);
int snprintf(char *buf, size_t maxlen, const char *fmt, ...);

/* Some static space for the stack */
char stack[8192];

/* Page of memory used to hold shared info*/
extern char shared_info[4096];
extern shared_info_t *HYPERVISOR_shared_info;

/* Other global variables */
char si_buff[SIBUFF_SZ];

/* Helper function for mapping shared info */
static shared_info_t *map_shared_info(unsigned long pa)
{
    int rc;

    if ((rc = HYPERVISOR_update_va_mapping((unsigned long)shared_info,
					__pte(pa | 7), UVMF_INVLPG))) {
        return (shared_info_t *)0;
	}

    return (shared_info_t *)shared_info;
}

/* Main kernel entry point, called by trampoline */
void start_kernel(start_info_t *start_info)
{
	struct timeval tv;
	time_t sec_old = 0;
	char buff[BUFF_SIZE];
	uint16_t cnt = 0;

	HYPERVISOR_console_io(CONSOLEIO_write,12,"Hello World\n");
	memcpy(si_buff, start_info, sizeof(*start_info)); // SIBUFF_SZ);
	HYPERVISOR_console_io(CONSOLEIO_write,18,"Start Info Copied\n");
	HYPERVISOR_shared_info = map_shared_info((unsigned long)si_buff);
	while(1) {
		gettimeofday(&tv, NULL);
		if ( ++cnt == 0 || tv.tv_sec != sec_old ) {
			snprintf(buff, BUFF_SIZE, "%ld\n", tv.tv_sec);
			HYPERVISOR_console_io(CONSOLEIO_write, strnlen(buff, BUFF_SIZE), buff);
			sec_old = tv.tv_sec;
		}
	}
}
