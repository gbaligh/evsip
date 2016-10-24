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

#include "evsip_glob.h"
#include "evsip_log.h"
#include "evsip_types.h"
#include "evsip_mod.h"
#include <sofia-sip/su_vector.h>
#include <string.h>

static su_vector_t *evsip_lmod = (su_vector_t *)0;

unsigned int evsip_mod_init(evsip_mod_t **mod)
{
   evsip_lmod = su_vector_create();
   if (evsip_lmod != 0) {
      return EVISP_ERROR;
   }
   return EVSIP_SUCCESS;
}

void evsip_mod_close(evsip_mod_t *mod)
{
   if (su_vector_empty(evsip_lmod) != 0) {
      return;
   }

   su_vector_destroy(evsip_lmod);
}

