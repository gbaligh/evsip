#ifndef EVSIP_MEM_H_
#define EVSIP_MEM_H_

/**
 * Defines the memory destructor handler, which is called when the reference
 * of a memory object goes down to zero
 *
 * @param data Pointer to memory object
 */
typedef void (evsip_mem_destroy_h)(void *data);


#endif /* EVSIP_MEM_H_ */
