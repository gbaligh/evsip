/*
EasyVision SIP Server (EvSip), a test Server for SIP protocol.
Copyright (C) 2016  Baligh Guesmi

This file is part of EvSip.

EvSip is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

EvSip is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
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
unsigned int evsip_cli_cmd_register(evsip_cli_cmd_t *pCmdCtx, unsigned int (*exec_f)(void *), void *arg);

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

void evsip_cli_cmd_destroy_all();


unsigned int evsip_cli_cmd_register_help_cmd(void);

#endif /* EVSIP_CLI_CMDLST_H_ */

//vim: noai:ts=2:sw=2
