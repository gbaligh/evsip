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
