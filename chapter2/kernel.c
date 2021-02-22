#include <stdint.h> 
#include <xen.h>

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

/* Some static space for the stack */
char stack[8192];


/* Main kernel entry point, called by trampoline */
void start_kernel(start_info_t *start_info)
{
	HYPERVISOR_console_io(CONSOLEIO_write,12,"Hello World\n");
	while(1);
}
