/*
 * evsip_endp.c
 *
 *  Created on: 22 déc. 2014
 *      Author: G545485
 */

/**
 * @file evsip_endp.c
 * @brief SIP end point controller
 */

#include <sofia-sip/su_vector.h>
#include "evsip_glob.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"
#include "evsip_endp.h"

/** @brief MAGIC for end point object */
#define EVSIP_ENDP_MAGIC		20150308

/** @brief Global cached end point */
static su_vector_t *gpEndpList = (su_vector_t *)0;

enum {
	EVSIP_ENDP_FLAG_ATTACHED	= (1 << 0), /**! To indicate that the object is cached*/
};

/**
 * @brief end point object
 */
struct evsip_endp_ctx {
	/** MAGIC */
	unsigned int magic;
	/** State */
	unsigned int state;
	/** Tools FLAGS */
	unsigned int flags;
	/** SIP information about end point*/
	struct {
		char *contact;
		char *host;
	} info;
	/** Authentication sent by end point */
	struct  {

	} credential;
};

unsigned int evsip_endp_create(evsip_endp_t **pEndp)
{
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)0;

	if (pEndp == (evsip_endp_t **)0) {
		return EVSIP_ERROR_BADPARAM;
	}

	_pEndp = (struct evsip_endp_ctx *)su_alloc(evSipGlobCtx->memPage, sizeof(struct evsip_endp_ctx));
	if (_pEndp == (struct evsip_endp_ctx *)0) {
		return EVSIP_ERROR_OUTOFRESOURCES;
	}

	memset(_pEndp, 0, sizeof(struct evsip_endp_ctx));

	_pEndp->magic = EVSIP_ENDP_MAGIC;

	*pEndp = _pEndp;

	return (EVSIP_SUCCESS);
}

unsigned int evsip_endp_attach(evsip_endp_t *pEndp)
{
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	if (_pEndp == (struct evsip_endp_ctx *)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (_pEndp->magic != EVSIP_ENDP_MAGIC) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "EndpMagic(%u)-[%u]", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (gpEndpList == (su_vector_t *) 0) {
		gpEndpList = su_vector_create(evSipGlobCtx->memPage, evsip_endp_destroy);
		if (gpEndpList == (su_vector_t *) 0) {
			return EVSIP_ERROR_OUTOFRESOURCES;
		}
	}

	if (su_vector_append(gpEndpList, _pEndp) != 0) {

	}

	_pEndp->flags |= EVSIP_ENDP_FLAG_ATTACHED;

	return EVSIP_SUCCESS;
}

unsigned int evsip_endp_dettach(evsip_endp_t *pEndp)
{
	int _i = 0;
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	if (_pEndp == (struct evsip_endp_ctx *)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (_pEndp->magic != EVSIP_ENDP_MAGIC) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "EndpMagic(%u)-[%u]", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (!(_pEndp->flags & EVSIP_ENDP_FLAG_ATTACHED)) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_WARNING, "Endp(%p) not in list", _pEndp);
		return EVSIP_ERROR_NOT_FOUND;
	}

	for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
		struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
		if ((_pEndpTmp) && (_pEndpTmp == _pEndp)) {
			su_vector_remove(gpEndpList, _i);
			_pEndp->flags &= ~EVSIP_ENDP_FLAG_ATTACHED;
			return EVSIP_SUCCESS;
		}
	}

	return EVSIP_ERROR_NOT_FOUND;
}

unsigned int evsip_endp_setContact(evsip_endp_t *pEndp, const char *pContact)
{
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	if (_pEndp == (struct evsip_endp_ctx *)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (_pEndp->magic != EVSIP_ENDP_MAGIC) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "EndpMagic(%u)-[%u]", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	_pEndp->info.contact = su_strdup(evSipGlobCtx->memPage, pContact);

	return (EVSIP_SUCCESS);
}

unsigned int evsip_endp_setHostIpv4(evsip_endp_t *pEndp, const char *pIpv4)
{
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	if (_pEndp == (struct evsip_endp_ctx *)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (_pEndp->magic != EVSIP_ENDP_MAGIC) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "EndpMagic(%u)-[%u]", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	_pEndp->info.host = su_strdup(evSipGlobCtx->memPage, pIpv4);

	return (EVSIP_SUCCESS);
}

void evsip_endp_dumpList()
{
	unsigned int _i = 0;

	EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_INFO, "Dump endp list: Number of elements %u", su_vector_len(gpEndpList));
	for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
		struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
		if (_pEndpTmp) {
			EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_INFO, "\tEndp[%u]: <sip:%s@%s>", _i, _pEndpTmp->info.contact, _pEndpTmp->info.host);
			break;
		}
	}
}

unsigned int evsip_endp_isAttached(evsip_endp_t *pEndp)
{
	unsigned int _i = 0;
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	if (_pEndp == (struct evsip_endp_ctx *)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return EVSIP_ERROR_BADPARAM;
	}

	if (_pEndp->magic != EVSIP_ENDP_MAGIC) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "EndpMagic(%u)-[%u]", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	if (!(_pEndp->flags & EVSIP_ENDP_FLAG_ATTACHED)) {
		return EVSIP_ERROR_NOT_FOUND;
	}

	for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
		struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
		if ((_pEndpTmp) && (_pEndpTmp == _pEndp)) {
			return EVSIP_SUCCESS;
		}
	}

	EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_WARNING, "Endp(%p) Attached flags set, but not found in list", _pEndp);
	_pEndp->flags &= ~EVSIP_ENDP_FLAG_ATTACHED;

	return EVSIP_ERROR_NOT_FOUND;
}

unsigned int evsip_endp_findByIpv4(const char *pIpv4, evsip_endp_t **pEndp)
{
	unsigned int _i = 0;
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)0;

	if (pEndp == (struct evsip_endp_ctx **)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return EVSIP_ERROR_BADPARAM;
	}

	if ((pIpv4 == (char *)0) || (pIpv4[0] == '\0')) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p) Host(%p)", _pEndp, pIpv4);
		return EVSIP_ERROR_BADPARAM;
	}

	for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
		struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
		if ((_pEndpTmp) && (strcmp(_pEndpTmp->info.host, pIpv4) == 0)) {
			_pEndp = _pEndpTmp;
			break;
		}
	}

	*pEndp = _pEndp;

	return (_pEndp != (struct evsip_endp_ctx *)0)?EVSIP_SUCCESS:EVSIP_ERROR_NOT_FOUND;
}

void evsip_endp_destroy(void *pEndp)
{
	struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	if (_pEndp == (struct evsip_endp_ctx *)0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p)", _pEndp);
		return;
	}

	if (_pEndp->magic != EVSIP_ENDP_MAGIC) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "EndpMagic(%u)-[%u]", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return;
	}

	su_free(evSipGlobCtx->memPage, _pEndp->info.contact);
	su_free(evSipGlobCtx->memPage, _pEndp->info.host);

	su_free(evSipGlobCtx->memPage, _pEndp);
}
