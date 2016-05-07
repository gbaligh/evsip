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
#ifndef EVSIP_SIG_H_
#define EVSIP_SIG_H_

/**
 *
 * @return
 */
unsigned int evsip_sig_init(su_root_t *pRoot, su_home_t *pHome);

/**
 * @return
 */
void evsip_sig_deinit(void);

#endif /* EVSIP_SIG_H_ */

//vim: noai:ts=2:sw=2
