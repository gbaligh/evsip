#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "evsip_cli.h"
#include "evsip_config.h"
#include "evsip_types.h"
#include "evsip_glob.h"
#include "evsip_core.h"
#include "evsip_sig.h"
#include "evsip_log.h"

/** @brief */
#define EVSIP_DEFAULT_CONFIG_FILE	"/etc/evsipconfig.conf"

/**
 *
 * @param[in] argc
 * @param[in] argv
 * @return EVSIP_SUCCESS
 */
static unsigned int evsip_usage(int argc, char *argv[])
{
    EVSIP_UNUSED_ARG(argc);

    fprintf(stdout, "%s [options]\n", argv[0]);

    return (EVSIP_SUCCESS);
}


/**
 *
 * @param[in] argc
 * @param[in] argv
 * @return  EVSIP_SUCCESS
 *          EVSIP_ERROR_BADPARAM
 */
static unsigned int evsip_args(int argc, char *argv[])
{
    int c = '0';

    if (argc > 1) {
        opterr = 0;
        while ((c = getopt (argc, argv, "hf:")) != -1) {
            switch (c) {
            case 'f':
                evSipGlobCtx->confFilePath = optarg;
                break;
            case 'h':
                evsip_usage(argc, argv);
                break;
            default:
                return (EVSIP_ERROR_BADPARAM);
            }
        }
    }

    return (EVSIP_SUCCESS);
}

/**
 * @brief Main Function for EvSip Server
 *
 * @param[in] argc
 * @param[in] argv
 * @return 	EXIT_SUCCESS on success
 * 			EXIT_FAILURE on failure
 */
int main(int argc, char *argv[])
{
    unsigned int _ret = EVSIP_SUCCESS;

    _ret = evsip_args(argc, argv);
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Bad arguments");
        goto LEAVE_ERROR;
    }

    _ret = evsip_cli_init();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not initialize commands line interface");
        goto LEAVE_ERROR;
    }

    _ret = evsip_config_init(evSipGlobCtx->pEvSipConfigCtx, evSipGlobCtx->confFilePath);
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not initialize configuration file");
        goto LEAVE_ERROR;
    }

    _ret = evsip_config_start_parse(evSipGlobCtx->pEvSipConfigCtx);
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_WARNING, "Error in configuration file, try to use default values...");
        //goto LEAVE_ERROR;
    }

    _ret = evsip_log_init();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not initialise LOG service");
        goto LEAVE_ERROR;
    }

    _ret = evsip_init();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not initialize EVSIP");
        goto LEAVE_ERROR;
    }

    _ret = evsip_sig_init(evSipGlobCtx->rootEventLoop, evSipGlobCtx->memPage);
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not initialize Signal handlers");
        goto LEAVE_ERROR;
    }

    _ret = evsip_cli_start();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not initialize CLI");
        goto LEAVE_ERROR;
    }

    _ret = evsip_start();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Can not start EVSIP");
        goto LEAVE_ERROR;
    }

    _ret = evsip_stop();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Problem during EVSIP stop");
        goto LEAVE_ERROR;
    }

    _ret = evsip_cli_stop();
    if (_ret != EVSIP_SUCCESS) {
        EVSIP_LOG(EVSIP_MAIN, EVSIP_LOG_ERROR, "Problem during CLI stop");
    }

    return (EXIT_SUCCESS);

LEAVE_ERROR:
    evsip_sig_deinit();
    evsip_cli_deinit();
    evsip_config_deinit(evSipGlobCtx->pEvSipConfigCtx);
    evsip_log_deinit();
		evsip_deinit();
    return (EXIT_FAILURE);
}

//vim: noai:ts=2:sw=2
