#include <sofia-sip/su.h>
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/su_alloc_stat.h>
#include "evsip_mem.h"
#include "evsip_types.h"

/** Defines a reference-counting memory object */
struct evsip_mem_str {
	su_home_t home[1]; /**< SOFIA-SIP memory manager */
};

static struct evsip_mem_str evsip_mem[1];

/**
 * Initialize the memory manager
 *
 * @param h SOFIA SIP memory manager to use
 *
 * @return EVSIP_SUCCESS on success
 */
unsigned int evsip_mem_init(su_home_t *h)
{
	if (su_home_init(h) != 0) {
		return (EVSIP_ERROR_UNKNOWN);
	}

	return (EVSIP_SUCCESS);
}

/**
 * Allocate a new reference-counted memory object
 *
 * @param size Size of memory object
 * @param dh   Optional destructor, called when destroyed
 *
 * @return Pointer to allocated object
 */
void *evsip_mem_alloc(size_t size, evsip_mem_destroy_h *dh)
{
	struct evsip_mem_str *m;

	m = su_alloc(evsip_mem->home, size);
	if (!m)
		return NULL;

	return ((void *)(m + 1));
}
