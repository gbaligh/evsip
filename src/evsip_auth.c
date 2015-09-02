#include <stdlib.h>
#include "evsip_types.h"
#include "evsip_glob.h"
#include "evsip_core.h"
#include "evsip_log.h"
#include <sofia-sip/auth_digest.h>


unsigned int evsip_auth_challenge(sip_authorization_t *auth)
{
    return EVSIP_SUCCESS;
}
