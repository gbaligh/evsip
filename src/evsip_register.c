/*
 * evsip_register.c
 *
 *  Created on: 19 déc. 2014
 *      Author: G545485
 */

#include <stdlib.h>
#include "evsip_glob.h"
#include "evsip_core.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"
#include "evsip_endp.h"
#include "evsip_register.h"
#include <sofia-sip/su_vector.h>

static inline unsigned int evsip_register_check(nua_t *nua, nua_handle_t *nh, sip_t const *sip)
{
	/* Check "Contact Header" existence and validity */
	if (!(sip->sip_contact && sip->sip_contact->m_url)) {
		EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "NO CONTACT!");
		nua_respond(nh, 400, "Missing Contact Header", NUTAG_WITH_THIS(nua), TAG_END());
		nua_handle_destroy(nh);
		return (EVSIP_ERROR_UNKNOWN);
	}

	/* Check if it is really a REQUEST */
    if (!sip || !sip->sip_request || !sip->sip_request->rq_method_name) {
    	EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "Malformed SIP Message");
        nua_respond(nh, SIP_500_INTERNAL_SERVER_ERROR, NUTAG_WITH_THIS(nua), TAG_END());
        nua_handle_destroy(nh);
        return (EVSIP_ERROR_UNKNOWN);
    }

	return (EVSIP_SUCCESS);
}

/**
 *
 * @param nua
 * @param nh
 * @param sip
 * @param tags
 * @return
 */
unsigned int evsip_register_handler(nua_t *nua, nua_handle_t *nh, sip_t const *sip, tagi_t tags[])
{
	const char *contact_host = (char *)0;
	uint64_t exptime = 300;
	char expbuf[35] = "";
	int _ret = EVSIP_SUCCESS;
	evsip_endp_t *_pEndp = (evsip_endp_t *)0;

	/* Unused ARGS */
	EVSIP_UNUSED_ARG(tags);

	/* Check REGISTER validity */
	_ret = evsip_register_check(nua, nh, sip);
	if (_ret != EVSIP_SUCCESS) {
		EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "Not acceptable message");
		return _ret;
	}

	/* Send "100 Trying" to stop retransmission */
	EVSIP_LOG(EVSIP_REG, EVSIP_LOG_DEBUG, "Sending 100 Trying");
	nua_respond(nh, SIP_100_TRYING, NUTAG_WITH_THIS(nua), TAG_END());

	/* Extract the host name or address in the "Contact Header" */
	if (sip && sip->sip_contact && sip->sip_contact->m_url) {
		contact_host = sip->sip_contact->m_url->url_host;
	}

	/* Extract "Expires Header" */
	if (sip->sip_expires) {
		exptime = sip->sip_expires->ex_delta;
	/* Extract "expires attribute" from "Contact Header" */
	} else if (sip->sip_contact && sip->sip_contact->m_expires) {
		exptime = atol(sip->sip_contact->m_expires);
	}
	snprintf(expbuf, sizeof(expbuf), "%ld", exptime);

	EVSIP_LOG(EVSIP_REG, EVSIP_LOG_DEBUG, "Host <%s> is trying to register for <%d> seconds using <%s> contact", contact_host, exptime, sip->sip_contact->m_url->url_user);

	/* TODO: Check if the endp is already registered */
	_ret = evsip_endp_findByIpv4(contact_host, &_pEndp);
	if ((_ret != EVSIP_SUCCESS) && (exptime > 0)) {
		_ret = evsip_endp_create(&_pEndp);
		if (_ret != EVSIP_SUCCESS) {
			EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "Can not create SIP end point.(%d)", _ret);
			goto EXIT_ERROR;
		}
	}

	if (exptime > 0) {
		_ret = evsip_endp_setHostIpv4(_pEndp, contact_host);
		if (_ret != EVSIP_SUCCESS) {
			EVSIP_LOG(EVSIP_REG, EVSIP_LOG_WARNING, "Can not set Host IPv4 address.(%d)", _ret);
		}

		_ret = evsip_endp_setContact(_pEndp, sip->sip_contact->m_url->url_user);
		if (_ret != EVSIP_SUCCESS) {
			EVSIP_LOG(EVSIP_REG, EVSIP_LOG_WARNING, "Can not set contact.(%d)", _ret);
		}

		_ret = evsip_endp_isAttached(_pEndp);
		if (_ret != EVSIP_SUCCESS) {
			_ret = evsip_endp_attach(_pEndp);
			if (_ret != EVSIP_SUCCESS) {
				EVSIP_LOG(EVSIP_REG, EVSIP_LOG_WARNING, "Can not add SIP end point to list.(%d)", _ret);
			}
		}
	}
	else if (_pEndp != (evsip_endp_t *)0){
		_ret = evsip_endp_dettach(_pEndp);
		if (_ret != EVSIP_SUCCESS) {
			EVSIP_LOG(EVSIP_REG, EVSIP_LOG_WARNING, "Can remove SIP end point from list.(%d)", _ret);
		}

		evsip_endp_destroy(_pEndp);
	}

	/* For test, Send "200 OK" */
	nua_respond(nh, SIP_200_OK, SIPTAG_CONTACT(sip->sip_contact),
			SIPTAG_EXPIRES_STR((const char *)expbuf),
			NUTAG_WITH_THIS(nua),
			TAG_END());

	return (EVSIP_SUCCESS);

EXIT_ERROR:
	nua_respond(nh, SIP_500_INTERNAL_SERVER_ERROR, NUTAG_WITH_THIS(nua), TAG_END());
	return _ret;
}
