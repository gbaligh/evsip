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
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <sofia-sip/su.h>
#define SU_LOG	evsip_log
#include <sofia-sip/su_log.h>
#include "evsip_types.h"
#include "evsip_log.h"

/**
 * @brief default log level
 */
#define EVSIP_LOG_DEF_LEVEL		9
/**
 * @brief Default Name if there is no name found for the module
 */
#define EVSIP_LOG_DEF_NAME		"evsip_log"
/**
 * @brief Envirenement paramter that contain LOG Level
 */
#define EVSIP_LOG_DEF_ENV		"EVSIP_LOG_DEBUG"
/**
 * @brief Set this macro to use SysLog
 */
#define EVSIP_LOG_USE_SYSLOG	0

/**
 * @brief
 */
su_log_t evSipLog[] = {
  {
    sizeof(su_log_t),
    EVSIP_LOG_DEF_NAME,
    EVSIP_LOG_DEF_ENV,
    EVSIP_LOG_DEF_LEVEL,
    EVSIP_LOG_MAX,
    0,
    (su_logger_f *)0,
    (void *)0
  }
};

/** @brief */
char const evsip_log_version[] = "1.0.0";

/** @brief */
const char *evsip_log_level_char[EVSIP_LOG_MAX] = {
  "NONE",
  "CRIT",
  "ERROR",
  "WARNING",
  "INFO",
  "DEBUG",
};

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site)  __attribute__((no_instrument_function));

void evsip_log_vprint(void *UNUSED(stream), char const *fmt, va_list ap)
{
  if (EVSIP_LOG_USE_SYSLOG) {
    vsyslog(0, fmt, ap);
  } else {
    vfprintf(stdout, fmt, ap);
  }
}

unsigned int evsip_log_init()
{
#if 0
  su_log_redirect(evSipLog, evsip_log_vprint, (void *)0);
  su_log_redirect(su_log_default, evsip_log_vprint, (void *)0);
  su_log_redirect(su_log_global, evsip_log_vprint, (void *)0);

  su_log_set_level(evSipLog, EVSIP_LOG_DEF_LEVEL);
  su_log_set_level(su_log_default, EVSIP_LOG_DEF_LEVEL);
  su_log_set_level(su_log_global, EVSIP_LOG_DEF_LEVEL);

  su_log_soft_set_level(evSipLog, EVSIP_LOG_DEF_LEVEL);
  su_log_soft_set_level(su_log_default, EVSIP_LOG_DEF_LEVEL);
  su_log_soft_set_level(su_log_global, EVSIP_LOG_DEF_LEVEL);
#endif
  return (EVSIP_SUCCESS);
}

void evsip_log_print(const char *file, const char *func, unsigned int line, unsigned int level, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  if (!EVSIP_LOG_USE_SYSLOG) {
    if (level != EVSIP_LOG_INFO)
      fprintf(stdout, "[%s] - %s[%d] - ", evsip_log_level_char[level], func, line);
  } else {
    syslog(LOG_MAKEPRI(LOG_DAEMON, level), "[%s]- ", evsip_log_level_char[level]);
  }
  evsip_log_vprint((void *)0, fmt, ap);
  va_end(ap);
}

void evsip_log_deinit()
{
  /* Nothing to do */
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) 
{
  //fprintf(stdout, "--> this_fn(%p),call_site(%p)\n", this_fn, call_site);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) 
{
  //fprintf(stdout, "<-- this_fn(%p),call_site(%p)\n", this_fn, call_site);
}

// vim: noai:ts=2:sw=2
