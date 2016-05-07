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
#include "evsip_glob.h"
#include "evsip_core.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"
#include "evsip_endp.h"
#include "evsip_register.h"

static inline unsigned int evsip_register_check(nua_t *nua, nua_handle_t *nh, sip_t const *sip)
{
    return (EVSIP_SUCCESS);
}

/**
 *
 * @param[in] nua
 * @param[in] nh
 * @param[in] sip
 * @param[in] tags
 * @return  EVSIP_SUCCESS
 *          EVSIP_ERROR_BADPARAM
 */
unsigned int evsip_register_handler(nua_t *nua, nua_handle_t *nh, sip_t const *sip, tagi_t tags[])
{
    const char *contact_host = (char *)0;
    uint64_t exptime = 300;
    char expbuf[35] = "";
    int _ret = EVSIP_SUCCESS;
    sip_authorization_t const *authorization = (sip_authorization_t *)0;
    evsip_endp_t *_pEndp = (evsip_endp_t *)0;

    /* Unused ARGS */
    EVSIP_UNUSED_ARG(tags);

    /* Check REGISTER validity */

    if (!sip || !sip->sip_request || !sip->sip_request->rq_method_name) {
        EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "Received an invalid packet!\n");
        goto EXIT_500_ERROR;
    }

    if (!sip->sip_to->a_url->url_user || !sip->sip_to->a_url->url_host) {
        EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "Can not do authorization without a complete header in REGISTER request");
        nua_respond(nh, SIP_401_UNAUTHORIZED, NUTAG_WITH_THIS(nua), TAG_END());
        return _ret;
    }

    /* TODO: RFC 3261 (10.3) */

    /* Send "100 Trying" to stop retransmission */
    EVSIP_LOG(EVSIP_REG, EVSIP_LOG_DEBUG, "Sending 100 Trying");
    nua_respond(nh, SIP_100_TRYING, NUTAG_WITH_THIS(nua), TAG_END());

    /* Extract the host name or address in the "Contact Header" */
    if (sip && sip->sip_contact && sip->sip_contact->m_url) {
        contact_host = sip->sip_contact->m_url->url_host;
    }

    if (sip->sip_authorization) {
        authorization = sip->sip_authorization;
    } else if (sip->sip_proxy_authorization) {
        authorization = sip->sip_proxy_authorization;
    }

    /* Send Authentication required */
    if (!authorization) {
        char auth[256];
        snprintf(auth, sizeof(auth), "Digest realm=\"EvSipServer\", nonce=\"%d\", algorithm=MD5, qop=\"auth\"", (int)random());
#if 0
        /* TODO: Save auth recod into database or create new auth module */
        nua_respond(nh, SIP_401_UNAUTHORIZED, SIPTAG_WWW_AUTHENTICATE_STR(auth), NUTAG_WITH_THIS(nua), TAG_END());
        return _ret;
#endif
    }

    /* Extract "Expires Header" */
    if (sip->sip_expires) {
        exptime = sip->sip_expires->ex_delta;
        /* Extract "expires attribute" from "Contact Header" */
    } else if (sip->sip_contact && sip->sip_contact->m_expires) {
        exptime = atol(sip->sip_contact->m_expires);
    }
    snprintf(expbuf, sizeof(expbuf), "%ld", exptime);

		EVSIP_LOG(EVSIP_REG, EVSIP_LOG_DEBUG, "handler(%p): Host <%s> is trying to register for <%d> seconds using <%s> contact", nh, contact_host, exptime, sip->sip_contact->m_url->url_user);

    /* TODO: Check if the endp is already registered */
    _ret = evsip_endp_findByIpv4(contact_host, &_pEndp);
    if ((_ret != EVSIP_SUCCESS) && (exptime > 0)) {
        _ret = evsip_endp_create(&_pEndp);
        if (_ret != EVSIP_SUCCESS) {
            EVSIP_LOG(EVSIP_REG, EVSIP_LOG_ERROR, "Can not create SIP end point.(%d)", _ret);
            goto EXIT_500_ERROR;
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
						_ret = evsip_endp_add(_pEndp);
            if (_ret != EVSIP_SUCCESS) {
                EVSIP_LOG(EVSIP_REG, EVSIP_LOG_WARNING, "Can not add SIP end point to list.(%d)", _ret);
            }
        }
    }
		else if (_pEndp != (evsip_endp_t *)0) {
				/* extract the Endp */
				_ret = evsip_endp_extract(_pEndp);
        if (_ret != EVSIP_SUCCESS) {
            EVSIP_LOG(EVSIP_REG, EVSIP_LOG_WARNING, "Can remove SIP end point from list.(%d)", _ret);
        }
				/* Free */
        evsip_endp_destroy(_pEndp);
    }

    /* For test, Send "200 OK" */
		EVSIP_LOG(EVSIP_REG, EVSIP_LOG_DEBUG, "handler(%p): Sending 200OK", nh);
    nua_respond(nh, SIP_200_OK, SIPTAG_CONTACT(sip->sip_contact),
                SIPTAG_EXPIRES_STR((const char *)expbuf),
                NUTAG_WITH_THIS(nua),
                TAG_END());

		evsip_endp_dumpList();

    return (EVSIP_SUCCESS);

EXIT_500_ERROR:
    nua_respond(nh, SIP_500_INTERNAL_SERVER_ERROR, NUTAG_WITH_THIS(nua), TAG_END());
    nua_handle_destroy(nh);
    return _ret;
}

//vim: noai:ts=2:sw=2
