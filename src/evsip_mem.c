#include <sofia-sip/su.h>
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/su_alloc_stat.h>
#include "evsip_mem.h"
#include "evsip_types.h"

/** Defines a reference-counting memory object */
struct evsip_mem_str {
  su_home_t home[1]; /**< SOFIA-SIP memory manager */
};

struct evsip_memHeader_str {
  unsigned int magic;
  uint32_t nbRefs;
  evsip_mem_destroy_h *dh;
};

static struct evsip_mem_str evsip_memCtx[1];

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

  evsip_memCtx->home = h;

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
  struct evsip_memHeader_str *mh;

  mh = su_alloc(evsip_memCtx->home, sizeof(struct evsip_memHeader_str) + size);
  if (!mh)
    return NULL;

  mh->nbRefs = 1;
  mh->dh = dh;

  return ((void *)(m + 1));
}

/**
 * Allocate a new reference-counted memory object. Memory is zeroed.
 *
 * @param size Size of memory object
 * @param dh   Optional destructor, called when destroyed
 *
 * @return Pointer to allocated object
 */
void *evsip_mem_zalloc(size_t size, evsip_mem_destroy_h *dh)
{
	void *p;

	p = evsip_mem_alloc(size, dh);
	if (!p)
		return NULL;

	memset(p, 0, size);

	return p;
}

/**
 * Reference a reference-counted memory object
 *
 * @param data Memory object
 *
 * @return Memory object (same as data)
 */
void *evsip_mem_ref(void *data)
{
	struct evsip_memHeader_str *m = (struct evsip_memHeader_str *)0;

	if (!data)
		return NULL;

	m = ((struct evsip_memHeader_str *)data) - 1;

	++m->nrefs;

	return data;
}

/**
 * Dereference a reference-counted memory object. When the reference count
 * is zero, the destroy handler will be called (if present) and the memory
 * will be freed
 *
 * @param data Memory object
 *
 * @return Always NULL
 */
void *evsip_mem_deref(void *data)
{
	struct evsip_memHeader_str *m = (struct evsip_memHeader_str *)0;

	if (!data)
		return NULL;

	m = ((struct evsip_memHeader_str *)data) - 1;

	if (--m->nrefs > 0)
		return NULL;

	if (m->dh)
		m->dh(data);

	if (m->nrefs > 0)
		return NULL;

	free(m);

	return NULL;
}

/**
 * Get number of references to a reference-counted memory object
 *
 * @param data Memory object
 *
 * @return Number of references
 */
uint32_t evsip_mem_nrefs(const void *data)
{
	struct evsip_memHeader_str *m = (struct evsip_memHeader_str *)0;

	if (!data)
		return 0;

	m = ((struct evsip_memHeader_str *)data) - 1;

	return m->nrefs;
}

//vim: noai:ts=2:sw=2
