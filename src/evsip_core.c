#include "evsip_glob.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"

/**
 *
 * @return
 */
unsigned int evsip_init()
{
	char uri[256];

	su_init();

	if (su_home_init(evSipGlobCtx->memPage) != 0) {
		EVSIP_LOG(EVSIP_CORE, EVSIP_LOG_ERROR, "Memory Pages creation failed !");
		return (EVSIP_ERROR_OUTOFRESOURCES);
	}

	evSipGlobCtx->rootEventLoop = su_root_create(evSipGlobCtx);
	if (evSipGlobCtx->rootEventLoop == (su_root_t *)0) {
		EVSIP_LOG(EVSIP_CORE, EVSIP_LOG_ERROR, "Event Loop creation failed !");
		return (EVSIP_ERROR_OUTOFRESOURCES);
	}

	snprintf(uri, sizeof(uri), "sip:%s:%d", evSipGlobCtx->pEvSipConfigCtx->localIpAddress, evSipGlobCtx->pEvSipConfigCtx->localPort);
	EVSIP_LOG(EVSIP_CORE, EVSIP_LOG_DEBUG, "URI:%s", uri);

	evSipGlobCtx->nua = nua_create(	evSipGlobCtx->rootEventLoop,
											evsip_evsofia_main,
											(nua_magic_t *)evSipGlobCtx,
											NUTAG_APPL_METHOD("REGISTER, INVITE, ACK, CANCEL"),
											NUTAG_ALLOW("REGISTER, INVITE, ACK, PRACK"),
											NUTAG_USER_AGENT(evSipGlobCtx->pEvSipConfigCtx->userAgent),
											NUTAG_URL(uri),
											TAG_END());
	if (evSipGlobCtx->nua == (nua_t *)0) {
		EVSIP_LOG(EVSIP_CORE, EVSIP_LOG_ERROR, "Sip User agent failed !");
		return (EVSIP_ERROR_OUTOFRESOURCES);
	}

	nua_set_params(evSipGlobCtx->nua,
			NUTAG_APPL_METHOD("REGISTER, INVITE, ACK, CANCEL"),
			NUTAG_ALLOW("REGISTER, INVITE, ACK, PRACK"),
			NUTAG_USER_AGENT(evSipGlobCtx->pEvSipConfigCtx->userAgent),
			NUTAG_URL(uri),
			TAG_END());

	return (EVSIP_SUCCESS);
}

/**
 *
 * @return
 */
unsigned int evsip_start()
{
	su_root_run(evSipGlobCtx->rootEventLoop);

	return (EVSIP_SUCCESS);
}

/**
 *
 * @return
 */
unsigned int evsip_stop()
{
	unsigned int _ret = EVSIP_SUCCESS;

	EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_INFO, "Stopping EvSip");

	su_root_break(evSipGlobCtx->rootEventLoop);

	if (su_root_release(evSipGlobCtx->rootEventLoop) != 0) {
		_ret = EVSIP_ERROR_UNKNOWN;
	}

	return (_ret);
}

/**
 *
 */
void evsip_deinit()
{
	nua_destroy(evSipGlobCtx->nua);
	evSipGlobCtx->nua = (nua_t *)0;
	su_root_destroy(evSipGlobCtx->rootEventLoop);
	evSipGlobCtx->rootEventLoop = (su_root_t *)0;
	su_home_deinit(evSipGlobCtx->memPage);
	su_deinit();
}
