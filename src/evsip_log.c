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

void evsip_log_print(const char *UNUSED(file), const char *UNUSED(func), unsigned int UNUSED(line), unsigned int level, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  if (!EVSIP_LOG_USE_SYSLOG) {
		fprintf(stdout, "\n[%s] - %s[%d] - ", evsip_log_level_char[level], func, line);
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

//vim: noai:ts=2:sw=2
