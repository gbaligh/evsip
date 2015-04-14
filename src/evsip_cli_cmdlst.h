/*
 * evsip_cli_cmdlst.h
 *
 *  Created on: 12 janv. 2015
 *      Author: G545485
 */

/**
 * @file evsip_cli_cmdlst.h
 * @brief
 */

#ifndef EVSIP_CLI_CMDLST_H_
#define EVSIP_CLI_CMDLST_H_

/**
 * Command Object
 */
typedef struct evsip_cli_cmd_str evsip_cli_cmd_t;

/**
 *
 * @param home
 * @param pCtx
 * @return
 */
unsigned int evsip_cli_cmd_create(su_home_t *home, evsip_cli_cmd_t **pCtx);

/**
 *
 * @param pCtx
 * @param cmd
 * @param help
 * @param privilege
 * @param mode
 * @return
 */
unsigned int evsip_cli_cmd_init(evsip_cli_cmd_t **pCtx,
								const char *cmd,
								const char *help,
								const unsigned int privilege,
								const unsigned int mode);

/**
 *
 * @param pCmdCtx
 * @return
 */
unsigned int evsip_cli_cmd_register(evsip_cli_cmd_t *pCmdCtx);

/**
 *
 * @param pCmd
 * @param pCmdCtx
 * @return
 */
unsigned int evsip_cli_cmd_find(const char *pCmd, evsip_cli_cmd_t **pCmdCtx);

/**
 *
 * @param pCmdCtx
 * @return
 */
const char *evsip_cli_cmd_str(evsip_cli_cmd_t *pCmdCtx);

/**
 *
 * @param pCmdCtx
 * @return
 */
unsigned int evsip_cli_cmd_execute(evsip_cli_cmd_t *pCmdCtx);

/**
 *
 * @param pCmdCtx
 */
void evsip_cli_cmd_destroy(evsip_cli_cmd_t *pCmdCtx);

#endif /* EVSIP_CLI_CMDLST_H_ */
