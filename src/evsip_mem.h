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

// vim: noai:ts=2:sw=2
