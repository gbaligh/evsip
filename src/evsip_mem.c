/*
EasyVision SIP Server (EvSip), a test Server for SIP protocol.
Copyright (C) 2016  Baligh Guesmi

This file is part of EvSip.

EvSip is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

EvSip is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <sofia-sip/su.h>
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/su_alloc_stat.h>
#include <string.h>
#include "evsip_mem.h"
#include "evsip_types.h"

#define EVSIP_MEM_MAGIC  0x15170516

/** Defines a reference-counting memory object */
struct evsip_mem_str {
  su_home_t *home; /**< SOFIA-SIP memory manager */
};

/**
 * @brief
 */
struct evsip_memHeader_str {
  unsigned int magic;
  uint32_t nbRefs;
  evsip_mem_destroy_h *dh;
};

/** @brief */
static struct evsip_mem_str evsip_memCtx[1];

/**
 * Initialize the memory manager
 *
 * @param[in] h SOFIA SIP memory manager to use
 *
 * @return EVSIP_SUCCESS on success
 */
unsigned int evsip_mem_init(su_home_t *h)
{
  if (su_home_init(h) != 0) {
    return (EVSIP_ERROR_UNKNOWN);
  }

  evsip_memCtx->home = h;

  return EVSIP_SUCCESS;
}

/**
 * Allocate a new reference-counted memory object
 *
 * @param[in] size Size of memory object
 * @param[in] dh   Optional destructor, called when destroyed
 *
 * @return Pointer to allocated object
 */
void *evsip_mem_alloc(size_t size, evsip_mem_destroy_h *dh)
{
  struct evsip_memHeader_str *mh = (struct evsip_memHeader_str *)0;

  mh = su_alloc(evsip_memCtx->home, sizeof(struct evsip_memHeader_str) + size);
  if (!mh)
    return NULL;

  mh->nbRefs = 1;
  mh->dh = dh;
  mh->magic = EVSIP_MEM_MAGIC;

  return ((void *)(mh + 1));
}

/**
 * Allocate a new reference-counted memory object. Memory is zeroed.
 *
 * @param[in] size Size of memory object
 * @param[in] dh   Optional destructor, called when destroyed
 *
 * @return Pointer to allocated object
 */
void *evsip_mem_zalloc(size_t size, evsip_mem_destroy_h *dh)
{
  void *p = NULL;

  p = evsip_mem_alloc(size, dh);
  if (!p)
    return NULL;

  memset(p, 0, size);

  return p;
}

/**
 * Reference a reference-counted memory object
 *
 * @param[in] data Memory object
 *
 * @return Memory object (same as data)
 */
void *evsip_mem_ref(void *data)
{
  struct evsip_memHeader_str *m = (struct evsip_memHeader_str *)0;

  if (!data)
    return NULL;

  m = ((struct evsip_memHeader_str *)data) - 1;

  if (m->magic != EVSIP_MEM_MAGIC) 
    return NULL;
  
  ++m->nbRefs;

  return data;
}

/**
 * Dereference a reference-counted memory object. When the reference count
 * is zero, the destroy handler will be called (if present) and the memory
 * will be freed
 *
 * @param[in] data Memory object
 *
 * @return NULL if there is no more reference.
 */
void *evsip_mem_deref(void *data)
{
  struct evsip_memHeader_str *m = (struct evsip_memHeader_str *)0;

  if (!data)
    return NULL;

  m = ((struct evsip_memHeader_str *)data) - 1;

  if (m->magic != EVSIP_MEM_MAGIC) 
    return NULL;
   
  if (--m->nbRefs > 0)
    return (void *)(m + 1);

  if (m->dh)
    m->dh(data);

  if (m->nbRefs > 0)
    return (void *)(m + 1);

  su_free(evsip_memCtx->home, m);

  return NULL;
}

/**
 * Get number of references to a reference-counted memory object
 *
 * @param[in] data Memory object
 *
 * @return Number of references
 */
uint32_t evsip_mem_nrefs(const void *data)
{
  struct evsip_memHeader_str *m = (struct evsip_memHeader_str *)0;

  if (!data)
    return 0;

  m = ((struct evsip_memHeader_str *)data) - 1;
  
  if (m->magic != EVSIP_MEM_MAGIC)
    return 0;
  
  return m->nbRefs;
}

// vim: noai:ts=2:sw=2
