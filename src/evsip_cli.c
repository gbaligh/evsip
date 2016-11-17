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
 * @file evsip_cli.c
 * @brief
 */

#include "evsip_glob.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"
#include "evsip_cli_cmdlst.h"
#include "evsip_cli.h"
#include <sofia-sip/su_vector.h>
#include <string.h>
#include <termios.h>

/**
 * Banner at start of CLI
 */
#define EVSIP_CLI_BANNER_STR		"===================== %s ====================\n"
/**
 * Normal prompt
 */
#define EVSIP_CLI_PROMPT_NCH_STR	">"
/**
 * Administrator prompt
 */
#define EVSIP_CLI_PROMPT_PCH_STR	"#"

/**
 *
 */
enum {
  EVSIP_CLI_STATE_INIT    	= (1 << 0), //!< EVSIP_CLI_STATE_INIT
  EVSIP_CLI_STATE_STARTED    	= (1 << 1), //!< EVSIP_CLI_STATE_STARTED
};

/**
 *
 */
struct evsip_cli_ctx_str {
  /** Terminal input configuration */
  struct termios evsip_saved_term[1];
  /** wait event from Sofia-sip event loop */
  su_wait_t input_wait[1];
  /** Out put file descriptor */
  int outFd;
  /** In put file descriptor */
  int inFd;
  /** Hostname to use */
  const char *hostname;
  /** Current prompt char used */
  char *prompt;
  /** State of the CLI */
  unsigned int state;
  /** Flags */
  unsigned int flags;
  /** Current Command */
  char cur_cmd[256];
  /** Current commands length */
  size_t cur_cmd_len;
  /** Registered command list RBTree */
  evsip_cli_cmd_t *cmdlist;
};

/**
 * Global Context of the CLI.
 * @note Only one CLI is handled
 */
struct evsip_cli_ctx_str evsip_cli_ctx = {
  .prompt = EVSIP_CLI_PROMPT_NCH_STR,
  .outFd = STDOUT_FILENO,
  .inFd = STDIN_FILENO,

  .cur_cmd = "",
  .cur_cmd_len = 0,
};


/**
 * Write prompt char to 'fdout', and write the buffer continent if 'clean' is not set.
 *
 * @param[in] fdout file descriptor to write to.
 * @param[in] clean read buffer before writing prompt.
 */
static void evsip_cli_prompt(int fdout, const unsigned int clean)
{
  char prompt[256] = "";
  snprintf(prompt, sizeof(prompt), "%s%s ", evsip_cli_ctx.hostname, evsip_cli_ctx.prompt);

  write(fdout, prompt, strlen(prompt));

  if (clean) {
    memset(evsip_cli_ctx.cur_cmd, 0, sizeof(evsip_cli_ctx.cur_cmd));
    evsip_cli_ctx.cur_cmd_len = 0;
  } else {
    write(fdout, evsip_cli_ctx.cur_cmd, evsip_cli_ctx.cur_cmd_len);
  }
}

/**
 *
 * @param Ctx
 * @param Wait
 * @param arg
 * @return
 */
static int evsip_cli_input_handler(	su_root_magic_t *Ctx, su_wait_t *Wait, su_wakeup_arg_t *arg)
{
	char c = 0;
	size_t size = 0;
	int fd = su_wait_socket(Wait);

	if (Ctx) {};
	if (arg) {};

	size = read(fd, &c, sizeof(c));
	if (size > 0) {
		switch (c) {
			case '?':
				if (write(evsip_cli_ctx.outFd, "\n", 1) > 0) { 
					evsip_cli_prompt(evsip_cli_ctx.outFd, 0);
				}
				break;
			case '\t': 
				{
					evsip_cli_cmd_t *_pCmdCtx = (evsip_cli_cmd_t *)0;
					write(evsip_cli_ctx.outFd, "\n", 1);
					if (evsip_cli_cmd_find(evsip_cli_ctx.cur_cmd, &_pCmdCtx) == EVSIP_SUCCESS) {
						fprintf(stdout, "%s\n", evsip_cli_cmd_str(_pCmdCtx));
					}
					evsip_cli_prompt(evsip_cli_ctx.outFd, 0);
				}
				break;
			case '\b':
				evsip_cli_ctx.cur_cmd[--evsip_cli_ctx.cur_cmd_len] = '\0';
				evsip_cli_prompt(evsip_cli_ctx.outFd, 0);
				break;
			case '\n':
				{
					evsip_cli_cmd_t *_pCmdCtx = (evsip_cli_cmd_t *)0;
					if (evsip_cli_ctx.cur_cmd_len > 0) {
						if (evsip_cli_cmd_find(evsip_cli_ctx.cur_cmd, &_pCmdCtx) == EVSIP_SUCCESS) {
							evsip_cli_cmd_execute(_pCmdCtx);
						} else {
							fprintf(stdout, "%s: Command not found\n", evsip_cli_ctx.cur_cmd);
						}
					}
					evsip_cli_prompt(evsip_cli_ctx.outFd, 1);
				}
				break;
			default:
				if (evsip_cli_ctx.cur_cmd_len < 255) {
					evsip_cli_ctx.cur_cmd[evsip_cli_ctx.cur_cmd_len++] = c;
				}
				break;
		}
	}

	return (EVSIP_SUCCESS);
}

/**
 *
 * @return
 * EVSIP_SUCCESS
 */
unsigned int evsip_cli_init()
{
  struct termios term[1];

  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
  setbuf(stderr, NULL);

  evsip_cli_ctx.inFd = STDIN_FILENO;
  evsip_cli_ctx.outFd = STDOUT_FILENO;

  /* Terminal configuration set */
  tcgetattr(evsip_cli_ctx.inFd, evsip_cli_ctx.evsip_saved_term);
  tcgetattr(evsip_cli_ctx.inFd, term);
  term->c_lflag &= ~(ICANON);
  term->c_cc[VEOL]  = '?';
  term->c_cc[VEOL2] = '\t';
  tcsetattr(evsip_cli_ctx.inFd, TCSAFLUSH, term);

  /* CLI configuration set */
  evsip_cli_ctx.hostname = evsip_version();
  evsip_cli_ctx.state |= EVSIP_CLI_STATE_INIT;

  return (EVSIP_SUCCESS);
}


/**
 *
 * @return
 * EVSIP_SUCCESS
 */
unsigned int evsip_cli_add_default_cmd()
{
  unsigned int _ret = EVSIP_SUCCESS;
  
  _ret = evsip_cli_cmd_register_help_cmd();
  if (_ret != EVSIP_SUCCESS) {
    return _ret;
  }

  return (EVSIP_SUCCESS);
}

/**
 *
 * @return
 * EVSIP_SUCCESS
 * EVSIP_ERROR_OUTOFRESOURCES
 */
unsigned int evsip_cli_start()
{
  int index = -1;

  fprintf(stdout, EVSIP_CLI_BANNER_STR, evsip_version());

  su_wait_init(evsip_cli_ctx.input_wait);

  if (su_wait_create(evsip_cli_ctx.input_wait, evsip_cli_ctx.inFd, SU_WAIT_ACCEPT|SU_WAIT_HUP|SU_WAIT_ERR|SU_WAIT_IN) != 0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "Create Wait event failed");
    return (EVSIP_ERROR_OUTOFRESOURCES);
  }

  index = su_root_register(evSipGlobCtx->rootEventLoop, evsip_cli_ctx.input_wait, evsip_cli_input_handler, &evsip_cli_ctx, 1);
  if (index < 0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "Registering Wait event failed");
    return (EVSIP_ERROR_OUTOFRESOURCES);
  }

  evsip_cli_add_default_cmd();

  evsip_cli_prompt(evsip_cli_ctx.outFd, 1);

  evsip_cli_ctx.state |= EVSIP_CLI_STATE_STARTED;

  return (EVSIP_SUCCESS);
}

/**
 *
 * @return
 * EVSIP_SUCCESS
 */
unsigned int evsip_cli_stop()
{
  EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_INFO, "Stopping Command Line Interface");

  su_root_unregister(evSipGlobCtx->rootEventLoop, evsip_cli_ctx.input_wait, evsip_cli_input_handler, NULL);

  su_wait_destroy(evsip_cli_ctx.input_wait);

  return (EVSIP_SUCCESS);
}

/**
 * Free all resources.
 */
void evsip_cli_deinit()
{
  evsip_cli_cmd_destroy_all();

  /* if the TTY was changed, retrive last conf and set it again */
  if (evsip_cli_ctx.state & EVSIP_CLI_STATE_INIT)
    tcsetattr(evsip_cli_ctx.inFd, TCSAFLUSH, evsip_cli_ctx.evsip_saved_term);
}

// vim: noai:ts=2:sw=2
