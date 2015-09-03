#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h> /* for mode definitions */

#include "evsip_config.h"
#include "evsip_glob.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"

/**
 *
 */
#define EVSIP_CONFIG_MAX_LINE	128
/**
 *
 */
#define EVSIP_CONFIG_DEF_FILE	"/etc/evsip.conf"
/**
 *
 */
#define EVSIP_CONFIG_DEF_USRAGENT	"EasyVisionSipTestServer(EvSip)"
/**
 *
 */
#define EVSIP_CONFIG_DEF_LOCAL_PORT	SIP_DEFAULT_PORT
/**
 *
 */
#define EVSIP_CONFIG_DEF_LOCAL_ADDR	"127.0.0.1"
/**
 *
 */
enum evsip_config_type_enum {
  EVSIP_CONIFG_TYPE_STRING64,//!< EVSIP_CONIFG_TYPE_STRING64
  EVSIP_CONIFG_TYPE_INTEGER  //!< EVSIP_CONIFG_TYPE_INTEGER
};

/**
 * @struct evsip_config_name_ptr_mapping_str
 * @brief Mapping configuration string with attribute in Configuration Object
 */
struct evsip_config_name_ptr_mapping_str {
  /** Option name */
	const char *name;
  /** Value type of the option to parse */
	enum evsip_config_type_enum valueType;
  /** Offset of mapped attribute in the configuration Object */
	size_t offset;
} evsip_config_mapping[] = {
  {"local_ip_address",  EVSIP_CONIFG_TYPE_STRING64,  offsetof(struct evsip_config_str, localIpAddress)},
  {"local_port",        EVSIP_CONIFG_TYPE_INTEGER,   offsetof(struct evsip_config_str, localPort)},
  {"user_agent",        EVSIP_CONIFG_TYPE_STRING64,  offsetof(struct evsip_config_str, userAgent)},
  {"max_reg_expire",    EVSIP_CONIFG_TYPE_INTEGER,   offsetof(struct evsip_config_str, maxRegExpires)},
  {"min_reg_expire",    EVSIP_CONIFG_TYPE_INTEGER,   offsetof(struct evsip_config_str, minRegExpires)}

};

/**
 * Strip whitespace chars off end of given string, in place.
 *
 * @param s
 * @return
 */
static inline char* evsip_config_rstrip(char* s)
{
  char* p = s + strlen(s);
  while (p > s && isspace((unsigned char)(*--p)))
    *p = '\0';
  return (s);
}

/**
 * Return pointer to first non-whitespace char in given string.
 *
 * @param s
 * @return
 */
static inline char* evsip_config_lstrip(const char* s)
{
  while (*s && isspace((unsigned char)(*s)))
    s++;
  return ((char*)s);
}

/**
 * Return pointer to first char c or ';' comment in given string, or pointer to
 * null at end of string if neither found. ';' must be prefixed by a whitespace
 * character to register as a comment.
 *
 * @param s
 * @param c
 * @return
 */
static char* evsip_config_find_char(const char* s, char c)
{
  int was_whitespace = 0;
  while (*s && *s != c && !(was_whitespace && *s == ';')) {
    was_whitespace = isspace((unsigned char)(*s));
    s++;
  }
  return ((char*)s);
}

/**
 *
 * @param name
 * @param value
 * @return
 */
static unsigned int evsip_config_keyval_handler(struct evsip_config_str *_pEvSipConfigCtx, char *name, char *value)
{
  uint32_t i = 0;

  if (_pEvSipConfigCtx == (struct evsip_config_str *)0) {
    return (EVSIP_ERROR_INVALID_HANDLE);
  }

  for (i = 0; i < sizeof(evsip_config_mapping)/sizeof(evsip_config_mapping[0]); ++i) {
    if (!strcmp(name, evsip_config_mapping[i].name)) {
      EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_DEBUG, "Match found %s: %s", name, value);
      switch (evsip_config_mapping[i].valueType) {
        case EVSIP_CONIFG_TYPE_STRING64: 
          {
            EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_DEBUG, "Setting config at offset %d with value %s", evsip_config_mapping[i].offset, value);
            memcpy((char *)_pEvSipConfigCtx + evsip_config_mapping[i].offset, value, EVSIP_CONFIG_MAX_VALUE);
            break;
          }
        case EVSIP_CONIFG_TYPE_INTEGER: 
          {
            int _V = atoi(value);
            EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_DEBUG, "Setting config at offset %d with value %d", evsip_config_mapping[i].offset, _V);
            memcpy((char *)_pEvSipConfigCtx + evsip_config_mapping[i].offset, &_V, sizeof(int));
            break;
          }
      }
      return (EVSIP_SUCCESS);
    }
  }
	return EVSIP_ERROR_UNKNOWN;
}

/**
 *
 * @param pEvSipConfigCtx
 * @return
 */
unsigned int evsip_config_start_parse(evsip_config_t *pEvSipConfigCtx)
{
  /* Uses a fair bit of stack (use heap instead if you need to) */
  char line[EVSIP_CONFIG_MAX_LINE] = "";
  char *start = (char *)0;
  char *end = (char *)0;
  char *name = (char *)0;
  char *value = (char *)0;
  int lineno = 0;
  struct evsip_config_str *_pEvSipConfigCtx = (struct evsip_config_str *)pEvSipConfigCtx;

  if (_pEvSipConfigCtx == (struct evsip_config_str *)0) {
    EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_ERROR, "NULL pointer");
    return (EVSIP_ERROR_INVALID_HANDLE);
  }

  if ((_pEvSipConfigCtx->filePath == (char *)0) || (_pEvSipConfigCtx->filePath[0] == '\0')) {
    EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_ERROR, "File not configured"); 
    return (EVSIP_ERROR_UNKNOWN);
  }

  _pEvSipConfigCtx->fd = fopen(_pEvSipConfigCtx->filePath, "r");
  if (_pEvSipConfigCtx->fd == NULL) {
    EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_ERROR, "Can not open file %s", _pEvSipConfigCtx->filePath);
    return (EVSIP_ERROR_INVALID_HANDLE);
  }

  /* Scan through file line by line */
  while (fgets(line, EVSIP_CONFIG_MAX_LINE, _pEvSipConfigCtx->fd) != NULL) {
    lineno++;
    start = line;
    start = evsip_config_lstrip(evsip_config_rstrip(start));

    if (*start && *start != ';') {
      /* Not a comment, must be a name[=:]value pair */
      end = evsip_config_find_char(start, '=');
      if (*end != '=') {
        end = evsip_config_find_char(start, ':');
      }
      if (*end == '=' || *end == ':') {
        *end = '\0';
        name = evsip_config_rstrip(start);
        value = evsip_config_lstrip(end + 1);
        end = evsip_config_find_char(value, '\0');
        if (*end == ';')
          *end = '\0';
        evsip_config_rstrip(value);

        /* Valid name[=:]value pair found, call handler */
        if (evsip_config_keyval_handler(_pEvSipConfigCtx, name, value) != EVSIP_SUCCESS) {
          EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_WARNING, "Error Parse configuration file line %d", lineno);
        }
      } else {
        EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_WARNING, "not key value format in configuration file line %d", lineno);
      }
    }
  }

	return EVSIP_SUCCESS;
}

/**
 *
 * @param pEvSipConfigCtx
 * @param pFilePath
 * @return
 */
unsigned int evsip_config_init(evsip_config_t *pEvSipConfigCtx, const char *pFilePath)
{
  struct evsip_config_str *_pEvSipConfigCtx = (struct evsip_config_str *)pEvSipConfigCtx;

  if (_pEvSipConfigCtx == (struct evsip_config_str *)0) {
    EVSIP_LOG(EVSIP_CONFIG, EVSIP_LOG_CRIT, "NULL pointer");
    return (EVSIP_ERROR_UNKNOWN);
  }

  /* Initialize default values */
  if ((pFilePath != NULL) && (pFilePath[0] != '\0')) {
    _pEvSipConfigCtx->filePath = pFilePath;
  } else {
    _pEvSipConfigCtx->filePath = EVSIP_CONFIG_DEF_FILE;
  }

  _pEvSipConfigCtx->localPort = EVSIP_CONFIG_DEF_LOCAL_PORT;

  strcpy(_pEvSipConfigCtx->localIpAddress, EVSIP_CONFIG_DEF_LOCAL_ADDR);

  strcpy(_pEvSipConfigCtx->userAgent, EVSIP_CONFIG_DEF_USRAGENT);

	return EVSIP_SUCCESS;
}

/**
 *
 * @param pEvSipConfigCtx
 * @return
 */
unsigned int evsip_config_deinit(evsip_config_t *pEvSipConfigCtx)
{
  struct evsip_config_str *_pEvSipConfigCtx = (struct evsip_config_str *)pEvSipConfigCtx;

  if (_pEvSipConfigCtx == (struct evsip_config_str *)0) {
    return (EVSIP_ERROR_INVALID_HANDLE);
  }

  if (_pEvSipConfigCtx->fd == NULL) {
    return (EVSIP_ERROR_INVALID_HANDLE);
  }

  fclose(_pEvSipConfigCtx->fd);

	return EVSIP_SUCCESS;
}

//vim: noai:ts=2:sw=2
