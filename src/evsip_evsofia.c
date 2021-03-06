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

#include "evsip_register.h"
#include "evsip_invite.h"

/**
 * @brief Sofia-Sip event handler
 * This the first call of all Sofia-Sip accured event.
 *
 * @param[in] event The event
 * @param[in] status SIP message status (1xx)
 * @param[in] phrase SIP message phrase (Trying)
 * @param[in] nua User agent context
 * @param[in] magic User context associated with User agent
 * @param[in] nh Network Transaction Handler context
 * @param[in] hmagic Network/Transaction user context
 * @param[in] sip SIP message parsed
 * @param[in] tags Sofia-Sip available TAGS in this SIP message
 */
void evsip_evsofia_main(nua_event_t   event,
      int           status,
      char const   *phrase,
      nua_t        *nua,
      nua_magic_t  *magic,
      nua_handle_t *nh,
      nua_hmagic_t *hmagic,
      sip_t const  *sip,
      tagi_t        tags[])
{
  EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "[%s] event from SofiaSip"
      " - status:%d"
      " - phrase:%s"
      " - nua:%p"
      " - magic:%p"
      " - handler:%p"
      " - handler magic: %p"
      " - sip:%p",
      nua_event_name(event),
      status, phrase, nua, magic, nh, hmagic, sip);

   if (sip) {
      const sip_from_t *pFromHeader = (sip_from_t *)0;
      tl_gets(tags, SIPTAG_FROM_REF(pFromHeader), TAG_END());
   }

  switch (event) {
    case nua_i_register:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "REGISTER received");
      evsip_register_handler(nua, nh, sip, tags);
      break;

    case nua_i_invite:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "INVITE received");
      evsip_invite_handler(nua, nh, sip, tags);
      break;

    case nua_i_update:
    case nua_i_ack:
    case nua_r_bye:
    case nua_r_cancel:
      break;

    case nua_i_state:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "state changed event %d:%s\n", event, phrase);
      break;

    case nua_i_error:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_ERROR, "error event %d:%s\n", event, phrase);
      break;

    case nua_r_respond:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "respond event %d:%s\n", event, phrase);
      break;

    case nua_r_shutdown:
      su_root_break(evSipGlobCtx->rootEventLoop);
      break;

    case nua_r_set_params:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "Set parameter SofiaSip: %s \n", status!=200?"KO":"OK");
      break;

    default:
      EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_ERROR, "unknown/not handled event %d:%s\n", event, phrase);
      break;
   }
}

// vim: noai:ts=2:sw=2
