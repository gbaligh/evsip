#ifndef EVSIP_MEM_H_
#define EVSIP_MEM_H_

/**
 * Defines the memory destructor handler, which is called when the reference
 * of a memory object goes down to zero
 *
 * @param data Pointer to memory object
 */
typedef void (evsip_mem_destroy_h)(void *data);



unsigned int evsip_mem_init(su_home_t *h);

void *evsip_mem_alloc(size_t size, evsip_mem_destroy_h *dh);


void *evsip_mem_zalloc(size_t size, evsip_mem_destroy_h *dh);

void *evsip_mem_ref(void *data);

void *evsip_mem_deref(void *data);

uint32_t evsip_mem_nrefs(const void *data);


#endif /* EVSIP_MEM_H_ */

//vim: noai:ts=2:sw=2
