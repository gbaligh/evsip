#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "evsip_types.h"
#include "evsip_log.h"
#include "evsip_core.h"
#include "evsip_glob.h"

static struct sigaction pEvSipSigAct[1];

/**
 *
 * @param signum
 */
static void evsip_sig_handler(int signum)
{
	EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_DEBUG, "SIGNAL %d caught using basic handler", signum);
}
/**
 *
 * @param signum
 * @param pInfo
 * @param pCtx
 */
static void evsip_sig_sigaction(int signum, siginfo_t *pInfo, void *pCtx)
{
	EVSIP_UNUSED_ARG(pInfo && pCtx);
	EVSIP_LOG(EVSIP_SIG, EVSIP_LOG_DEBUG, "SIGNAL %d caught using advanced signal handler", signum);
	su_root_break(evSipGlobCtx->rootEventLoop);
}
/**
 *
 * @return
 */
unsigned int evsip_sig_init()
{
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

	return (EVSIP_SUCCESS);
}
/**
 *
 */
void evsip_sig_deinit()
{

}
