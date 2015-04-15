#include "evsip_glob.h"

#include <sofia-sip/sofia_features.h>

/**
 * @brief Global EvSip Server Context
 */
evsip_glob_t evSipGlobCtx[1];

/**
 *
 * @return
 */
const char *evsip_version(void)
{
	return (sofia_sip_name_version);
}

