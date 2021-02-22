#ifndef __HYPERCALL_DBG_X86_64_H__
#define __HYPERCALL_DBG_X86_64_H__

#include <xen.h>

#define __STR(x) #x
#define STR(x) __STR(x)

shared_info_t *HYPERVISOR_shared_info;

typedef struct { unsigned long pte; } pte_t;

#define __pte(x) ((pte_t) { (x) } )


static inline int HYPERVISOR_update_va_mapping(unsigned long va, 
								pte_t new_val, unsigned long flags)
{
        return _hypercall3(int, update_va_mapping, va, new_val.pte, flags);
}

#endif
