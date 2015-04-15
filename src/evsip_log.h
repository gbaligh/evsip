#ifndef EVSIP_LOG_H_
#define EVSIP_LOG_H_

enum {
  EVSIP_LOG_NONE,
  EVSIP_LOG_CRIT,
  EVSIP_LOG_ERROR,
  EVSIP_LOG_WARNING,
  EVSIP_LOG_INFO,
  EVSIP_LOG_DEBUG,
  EVSIP_LOG_MAX
};

/**
 * @brief EVSIP_LOG
 */
#define EVSIP_LOG(__MODULE__, __LEVEL__, __FMT__, __ARGS__...) \
  do { \
    evsip_log_print(__FILE__, __FUNCTION__, __LINE__, __LEVEL__, __FMT__ "\n", ##__ARGS__); \
  } while (0)

/**
 *
 * @return
 */
unsigned int evsip_log_init(void);

/**
 *
 * @param file
 * @param func
 * @param line
 * @param level
 * @param fmt
 */
void evsip_log_print(const char *file, const char *func, unsigned int line, unsigned int level, const char *fmt, ...);

/**
 *
 */
void evsip_log_deinit(void);

#endif /* EVSIP_LOG_H_ */

//vim: noai:ts=2:sw=2
