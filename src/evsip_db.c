
#include "evsip_types.h"
#include "evsip_glob.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_db.h"
#include <cdb.h>

#define EVSIP_DB_BASE               "evsip"

#define EVSIP_DB_DATABASE_PATH      "/tmp/" EVSIP_DB_BASE

struct evsip_db_str {
    /* Magic */
    unsigned int magic;
    /* DataBase Path */
    char *dbPath;
    /* Table */
    char *table;
};

unsigned int evsip_db_init(evsip_db_t **ppCtx, const char *pModule)
{
    struct evsip_db_str *_pCtx = (struct evsip_db_str*)0;
    int _fd;
    char pDbPath[256] = "";

    snprintf(pDbPath, sizeof(pDbPath), "%s/%s", EVSIP_DB_DATABASE_PATH, pModule);

    _fd = open(pDbPath, O_RDONLY);
    if (_fd < 0) {
        /* Database not found, create a new one */
        _fd = open(pDbPath, O_RDWR | O_CREAT);
    }

    if (_fd < 0) {
        return EVSIP_ERROR_ILLEGAL_ACTION;
    }

    close(_fd);

    *ppCtx = _pCtx;
    return EVSIP_SUCCESS;
}

unsigned int evsip_db_insert_into(char *pTable,
                                  const void *key, unsigned klen,
                                  const void *val, unsigned vlen)
{
    int _fd;

    if (!pTable) {
        return EVSIP_ERROR_BADPARAM;
    }

    return EVSIP_SUCCESS;
}

void *evsip_db_find_recod(char *pTable)
{
    return ((void *)0);
}


