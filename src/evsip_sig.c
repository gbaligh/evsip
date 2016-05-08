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
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "evsip_types.h"
#include "evsip_log.h"
#include "evsip_core.h"
#include "evsip_glob.h"

/** @brief */
static struct sigaction pEvSipSigAct[1];

static struct evsip_sig_str {
    su_home_t *home;
    su_root_t *root;
    _su_task_r task;
    struct sigaction *action;
} pEvSipSigCtx[1];

/**
 * @brief Handler for all Signal basic
 *
 * @param[in] signum
 */
static void evsip_sig_handler(int signum)
{
    EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_DEBUG, "SIGNAL %d caught using basic handler", signum);
}

/**
 * @brief evsip_sig_task_clbk
 * @param pRef
 * @return
 */
static int evsip_sig_task_clbk(void *pRef)
{
    su_root_break(pEvSipSigCtx->root);
    return 0;
}

/**
 * @brief Handler for all Signal with info
 *
 * @param[in] signum
 * @param[in] pInfo
 * @param[in] pCtx
 */
static void evsip_sig_sigaction(int signum, siginfo_t *pInfo, void *pCtx)
{
    int _retVal = 0;
    EVSIP_UNUSED_ARG(pInfo && pCtx);
    EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_DEBUG, "SIGNAL %d caught using advanced signal handler", signum);
		/* execute the callback function in the main root thread */
		su_task_execute(pEvSipSigCtx->task, evsip_sig_task_clbk, (void *)0, &_retVal);
}

/**
 * @brief Initialize the Signal handler module
 *
 * @return
 * EVSIP_SUCCESS
 * EVSIP_ERROR_UNINITIALIZED
 */
unsigned int evsip_sig_init(su_root_t *pRoot, su_home_t *pHome)
{
    if ((!pRoot) || (!pHome)) {
        return EVSIP_ERROR_BADPARAM;
    }
  /*
   * specifies the action to be associated with signum and may
   * be SIG_DFL for the default action, SIG_IGN to ignore this signal, or
   * a pointer to a signal handling function.  This function receives the
   * signal number as its only argument.
   */
    pEvSipSigAct->sa_handler = evsip_sig_handler;
    /*
   * If SA_SIGINFO is specified in sa_flags, then sa_sigaction (instead of
   * sa_handler) specifies the signal-handling function for signum.  This
   * function receives the signal number as its first argument, a pointer
   * to a siginfo_t as its second argument and a pointer to a ucontext_t
   * (cast to void *) as its third argument.  (Commonly, the handler
   * function doesn't make any use of the third argument.  See
   * getcontext(3) for further information about ucontext_t.)
   */
    pEvSipSigAct->sa_sigaction = evsip_sig_sigaction;
    /* The signal handler takes three arguments, not one.  In
   * this case, sa_sigaction should be set instead of
   * sa_handler.  This flag is meaningful only when
   * establishing a signal handler. */
    pEvSipSigAct->sa_flags = SA_SIGINFO | SA_RESTART;

    sigfillset(&pEvSipSigAct->sa_mask);

    if (sigaction(SIGINT, pEvSipSigAct, NULL) < 0) {
        EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_ERROR, "Setting SIGINT failure");
        return (EVSIP_ERROR_UNINITIALIZED);
    }
    if (sigaction(SIGTERM, pEvSipSigAct, NULL) < 0) {
        EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_ERROR, "Setting SIGKTERM failure");
        return (EVSIP_ERROR_UNINITIALIZED);
    }

    pEvSipSigCtx->action = pEvSipSigAct;
    pEvSipSigCtx->home = pHome;
    pEvSipSigCtx->root = pRoot;
		pEvSipSigCtx->task = su_root_task(pRoot);

		return EVSIP_SUCCESS;
}

/**
 * @brief
 */
void evsip_sig_deinit()
{
    /* nothing to do right now */
}

//vim: noai:ts=2:sw=2
