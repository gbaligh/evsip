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
#ifndef EVSIP_INVITE_H_
#define EVSIP_INVITE_H_

/**
 *
 * @param nua
 * @param nh
 * @param sip
 * @param tags
 * @return
 */
unsigned int evsip_invite_handler(nua_t *nua, nua_handle_t *nh, sip_t const *sip, tagi_t tags[]);

#endif /* EVSIP_REGISTER_H_ */

// vim: noai:ts=2:sw=2
