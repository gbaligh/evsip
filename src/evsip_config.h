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

#ifndef EVSIP_CONFIG_H_
#define EVSIP_CONFIG_H_

/**
 *
 */
#define EVSIP_CONFIG_MAX_NAME	64
/**
 *
 */
#define EVSIP_CONFIG_MAX_VALUE	64

/**
 * @brief EvSip Configuration
 */
struct evsip_config_str {
	/** MAGIC */
	unsigned int 	magic;
	/** Configuration file descriptor */
	FILE* 			fd;
	/** Configuration file path */
	const char*		filePath;
	/** Option Local IP Address */
	char 			localIpAddress[EVSIP_CONFIG_MAX_VALUE];
	/** Option Local Port */
	int 			localPort;
	/** Option User Agent */
	char 			userAgent[EVSIP_CONFIG_MAX_VALUE];
	/** Option Max Expires to accept */
	int				maxRegExpires;
	/** Option Min Expires to accept */
	int				minRegExpires;
};

/**
 *
 */
typedef struct evsip_config_str evsip_config_t;

/**
 *
 * @param pEvSipConfigCtx
 * @param pFilePath
 * @return
 */
unsigned int evsip_config_init(evsip_config_t *pEvSipConfigCtx, const char *pFilePath);

/**
 *
 * @param pEvSipConfigCtx
 * @return
 */
unsigned int evsip_config_start_parse(evsip_config_t *pEvSipConfigCtx);

/**
 *
 * @return
 */
unsigned int evsip_config_deinit(evsip_config_t *pEvSipConfigCtx);

#endif /* EVSIP_CONFIG_H_ */

//vim: noai:ts=2:sw=2
