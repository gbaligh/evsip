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

#include <stdlib.h>
#include "evsip_types.h"
#include "evsip_glob.h"
#include "evsip_core.h"
#include "evsip_log.h"
#include <sofia-sip/auth_digest.h>


unsigned int evsip_auth_challenge(sip_authorization_t *auth)
{
    return EVSIP_SUCCESS;
}

// vim: noai:ts=2:sw=2
