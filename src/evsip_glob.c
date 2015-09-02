#include "evsip_glob.h"

#include <sofia-sip/sofia_features.h>

/**
 * @brief Global EvSip Server Context
 */
evsip_glob_t evSipGlobCtx[1];

const char *evsip_version_name = "EvSip";

/**
 *
 * @return EvSip version
 */
const char *evsip_version(void)
{
  return evsip_version_name;//(sofia_sip_name_version);
}

//vim: noai:ts=2:sw=2
