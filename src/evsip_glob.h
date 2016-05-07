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

#ifndef EVSIP_GLOB_H_
#define EVSIP_GLOB_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sofia-sip/su.h>
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/su_wait.h>
#include <sofia-sip/su_tag.h>
#include <sofia-sip/su_tag_class.h>
#include <sofia-sip/su_tag_inline.h>
#include <sofia-sip/su_tag_io.h>
#include <sofia-sip/su_tagarg.h>
#include <sofia-sip/sofia_features.h>
#include <sofia-sip/sip_protos.h>
#include <sofia-sip/sip_status.h>
#include <sofia-sip/sip_extra.h>
#include <sofia-sip/sip_parser.h>
#include <sofia-sip/sdp.h>
#include <sofia-sip/msg_addr.h>
#include <sofia-sip/msg_header.h>
#include <sofia-sip/msg_parser.h>
#include <sofia-sip/msg.h>
#include <sofia-sip/nua.h>

#include "evsip_config.h"
/**
 * @brief 	Global struct contains all application configuration
 * @struct 	evsip_glob_str
 */
struct evsip_glob_str {
  /** Memory manager */
  su_home_t 	memPage[1];
  /** Event Loop for Sofia-SIP */
  su_root_t 	*rootEventLoop;
  /** User Agent SIP for Sofia-SIP */
  nua_t		*nua;
  /** Program argument Configuration file path */
  const char *confFilePath;
  /** Configuration Object */
  evsip_config_t pEvSipConfigCtx[1];
};

/**
 * @typedef evsip_glob_t
 */
typedef struct evsip_glob_str evsip_glob_t;

/**
 *
 */
extern evsip_glob_t evSipGlobCtx[];

/**
 *
 * @return
 */
const char *evsip_version(void);

#endif /* EVSIP_GLOB_H_ */

//vim: noai:ts=2:sw=2
