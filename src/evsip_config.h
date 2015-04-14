/*
 * evsip_config.h
 *
 *  Created on: 6 déc. 2014
 *      Author: G545485
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
