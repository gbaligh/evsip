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
    evsip_log_print(__FILE__, __FUNCTION__, __LINE__, __LEVEL__, "%s - " __FMT__ "\n", "["#__MODULE__"]", ##__ARGS__); \
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

// vim: noai:ts=2:sw=2
