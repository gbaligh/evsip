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

/** @brief */
#define EVSIP_CTX_CHECK_AND_MAGIC(_CTX_, _MAGIC_, _CODERET_, _MSG_) \
	do { \
		if ((_CTX_) == (evsip_endp_t *)0) { \
			EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, _MSG_); \
			_CODERET_; \
		} \
		\
		if ((_CTX_)->magic != _MAGIC_) { \
			EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, _MSG_); \
			_CODERET_; \
		} \
	} while (0)

/** @brief Global cached end point */
static su_vector_t *gpEndpList = (su_vector_t *)0;

/** @brief */
enum {
	EVSIP_ENDP_FLAG_ADDED	= (1 << 0), /**! To indicate that the object is cached*/
	EVSIP_ENDP_FLAG_REGISTRED = (1 << 1)
};

/**
 * @brief end point object
 */
struct evsip_endp_ctx {
  /** MAGIC */
  unsigned int magic;
	/** Memory Pool */
	su_home_t home[1];
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


unsigned int evsip_endp_init()
{

	return EVSIP_SUCCESS;
}

/**
 * @brief evsip_endp_create
 * @param pEndp
 * @return
 */
unsigned int evsip_endp_create(evsip_endp_t **pEndp)
{
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)0;

  if (pEndp == (evsip_endp_t **)0) {
    return EVSIP_ERROR_BADPARAM;
  }

	_pEndp = (struct evsip_endp_ctx *)su_home_clone(evSipGlobCtx->memPage, sizeof(struct evsip_endp_ctx));
  if (_pEndp == (struct evsip_endp_ctx *)0) {
    return EVSIP_ERROR_OUTOFRESOURCES;
  }

  memset(_pEndp, 0, sizeof(struct evsip_endp_ctx));

  _pEndp->magic = EVSIP_ENDP_MAGIC;

  *pEndp = _pEndp;

	return EVSIP_SUCCESS;
}

/**
 * @brief evsip_endp_add
 * @param pEndp
 * @return
 */
unsigned int evsip_endp_add(evsip_endp_t *pEndp)
{
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	EVSIP_CTX_CHECK_AND_MAGIC(_pEndp, EVSIP_ENDP_MAGIC, return EVSIP_ERROR_INVALID_HANDLE, "Error handler Endp");

	if (gpEndpList == (su_vector_t *) 0) {
		gpEndpList = su_vector_create(_pEndp->home, evsip_endp_destroy);
		if (gpEndpList == (su_vector_t *) 0) {
			return EVSIP_ERROR_OUTOFRESOURCES;
		}
	}

	if (su_vector_append(gpEndpList, _pEndp) != 0) {
		EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_ERROR, "Endp(%p) could not added", _pEndp->magic, EVSIP_ENDP_MAGIC);
		return EVSIP_ERROR_INSUFFICIENT_BUFFER;
  }

	_pEndp->flags |= EVSIP_ENDP_FLAG_ADDED;

  return EVSIP_SUCCESS;
}

/**
 * @brief evsip_endp_extract
 * @param pEndp
 * @return
 */
unsigned int evsip_endp_extract(evsip_endp_t *pEndp)
{
  int _i = 0;
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	EVSIP_CTX_CHECK_AND_MAGIC(_pEndp, EVSIP_ENDP_MAGIC, return EVSIP_ERROR_INVALID_HANDLE, "Error handler Endp");

	if (!(_pEndp->flags & EVSIP_ENDP_FLAG_ADDED)) {
    EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_WARNING, "Endp(%p) not in list", _pEndp);
    return EVSIP_ERROR_NOT_FOUND;
  }

  for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
    struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
    if ((_pEndpTmp) && (_pEndpTmp == _pEndp)) {
      su_vector_remove(gpEndpList, _i);
			_pEndp->flags &= ~EVSIP_ENDP_FLAG_ADDED;
      return EVSIP_SUCCESS;
    }
  }

  return EVSIP_ERROR_NOT_FOUND;
}

/**
 * @brief evsip_endp_setContact
 * @param pEndp
 * @param pContact
 * @return
 */
unsigned int evsip_endp_setContact(evsip_endp_t *pEndp, const char *pContact)
{
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	EVSIP_CTX_CHECK_AND_MAGIC(_pEndp, EVSIP_ENDP_MAGIC, return EVSIP_ERROR_INVALID_HANDLE, "Error handler Endp");

	_pEndp->info.contact = su_strdup(_pEndp->home, pContact);

	return EVSIP_SUCCESS;
}

/**
 * @brief evsip_endp_setHostIpv4
 * @param pEndp
 * @param pIpv4
 * @return
 */
unsigned int evsip_endp_setHostIpv4(evsip_endp_t *pEndp, const char *pIpv4)
{
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	EVSIP_CTX_CHECK_AND_MAGIC(_pEndp, EVSIP_ENDP_MAGIC, return EVSIP_ERROR_INVALID_HANDLE, "Error handler Endp");

	_pEndp->info.host = su_strdup(_pEndp->home, pIpv4);

	return EVSIP_SUCCESS;
}

/**
 * @brief evsip_endp_dumpList
 */
void evsip_endp_dumpList()
{
  unsigned int _i = 0;

	EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_INFO, "==== Dump Endp list: Number of elements %u ===", su_vector_len(gpEndpList));
  for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
    struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
    if (_pEndpTmp) {
      EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_INFO, "\tEndp[%u]: <sip:%s@%s>", _i, _pEndpTmp->info.contact, _pEndpTmp->info.host);
    }
  }
	EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_INFO, "==== Dump Endp list: End ===", su_vector_len(gpEndpList));
}

/**
 * @brief evsip_endp_isAttached
 * @param pEndp
 * @return
 */
unsigned int evsip_endp_isAttached(evsip_endp_t *pEndp)
{
  unsigned int _i = 0;
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	EVSIP_CTX_CHECK_AND_MAGIC(_pEndp, EVSIP_ENDP_MAGIC, return EVSIP_ERROR_INVALID_HANDLE, "Error handler Endp");

	if (!(_pEndp->flags & EVSIP_ENDP_FLAG_ADDED)) {
    return EVSIP_ERROR_NOT_FOUND;
  }

  for (_i = 0; _i < su_vector_len(gpEndpList); ++_i) {
    struct evsip_endp_ctx *_pEndpTmp = (struct evsip_endp_ctx *)su_vector_item(gpEndpList, _i);
    if ((_pEndpTmp) && (_pEndpTmp == _pEndp)) {
      return EVSIP_SUCCESS;
    }
  }

  EVSIP_LOG(EVSIP_ENDP, EVSIP_LOG_WARNING, "Endp(%p) Attached flags set, but not found in list", _pEndp);
	_pEndp->flags &= ~EVSIP_ENDP_FLAG_ADDED;

  return EVSIP_ERROR_NOT_FOUND;
}

/**
 * @brief evsip_endp_findByIpv4
 * @param pIpv4
 * @param pEndp
 * @return
 */
unsigned int evsip_endp_findByIpv4(const char *pIpv4, evsip_endp_t **pEndp)
{
  unsigned int _i = 0;
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)0;

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

/**
 * @brief evsip_endp_destroy
 * @param pEndp
 */
void evsip_endp_destroy(void *pEndp)
{
  struct evsip_endp_ctx *_pEndp = (struct evsip_endp_ctx *)pEndp;

	EVSIP_CTX_CHECK_AND_MAGIC(_pEndp, EVSIP_ENDP_MAGIC, return, "Error handler Endp");

	su_home_unref(_pEndp->home);
}

//vim: noai:ts=2:sw=2
