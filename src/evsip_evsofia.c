/*
 * evsip_hsofia.c
 *
 *  Created on: 21 nov. 2014
 *      Author: G545485
 */

#include "evsip_glob.h"
#include "evsip_log.h"
#include "evsip_types.h"

#include "evsip_register.h"

/**
 *
 * @param event
 * @param status
 * @param phrase
 * @param nua
 * @param magic
 * @param nh
 * @param hmagic
 * @param sip
 * @param tags
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
	EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "Event from SofiaSIP %s", nua_event_name(event));
	EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "status:%d"
					" - phrase:%s"
					" - nua:%p"
					" - magic:%p"
					" - handler:%p"
					" - handler magic: %p"
					" - sip:%p",
					status, phrase, nua, magic, nh, hmagic, sip);

	switch (event) {

	case nua_i_register:
		evsip_register_handler(nua, nh, sip, tags);
		break;

	case nua_i_invite:
	case nua_i_update:
	case nua_i_ack:
	case nua_r_bye:
	case nua_r_cancel:
		break;

	case nua_i_state:
		break;

	case nua_i_error:
		nua_handle_destroy(nh);
		break;

	case nua_r_respond:
		EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "respond event %d:%s\n", event, phrase);
		break;

	case nua_r_shutdown:
		su_root_break(evSipGlobCtx->rootEventLoop);
		break;

	case nua_r_set_params:
		EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_DEBUG, "Sofiasip initialized\n", event, phrase);
		break;

	default:
		EVSIP_LOG(EVSIP_SOFIA, EVSIP_LOG_ERROR, "unknown/not handled event %d:%s\n", event, phrase);
		nua_respond(nh, status, phrase, TAG_END());
		break;
	}
}

