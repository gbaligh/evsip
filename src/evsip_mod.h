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
#ifndef _EVSIP_MOD_H_
#define _EVSIP_MOD_H_

/**
 * @def EVSIP_MOD_PRE
 * Module prefix
 */
#define EVSIP_MOD_PRE	""

/**
 * @def EVSIP_MOD_EXT
 * Module extension
 */
#define EVSIP_MOD_EXT	".so"

/**
 * @def EVSIP_EXPORT_SYM
 * Symbol to enable exporting of functions from a module
 */
#define EVSIP_EXPORT_SYM

/*******************************************************
 * API
 ******************************************************/
/**
 * @brief Defines the module initialisation handler
 */
typedef unsigned int (evsip_mod_init_f)(void);

/**
 * @brief Defines the module close handler
 */
typedef unsigned int (evsip_mod_close_f)(void);

/**
 * @brief
 */
typedef enum evsip_mod_type_enu {
	EVSIP_MOD_TYPE_SOMETHING,
	EVSIP_MOD_TYPE_UNKNOWN,
} evsip_mod_type_t;

/**
 * @brief
 */
typedef struct evsip_mod_info_str {
	const char *name;
	const evsip_mod_type_t type;
	evsip_mod_init_f *init;
	evsip_mod_close_f *close;
} evsip_mod_info_t;

/*******************************************************
 * Module Manager
 ******************************************************/
/**
 * @brief Module context
 */
typedef struct evsip_mod_str evsip_mod_t;

/**
 * @brief evsip_mod_init
 * @param mod
 * @return
 */
unsigned int evsip_mod_init(evsip_mod_t **mod);

/**
 * @brief evsip_mod_ext_load
 * @param mod
 * @param name
 * @return
 */
unsigned int evsip_mod_ext_load(evsip_mod_t *mod, const char *name);

/**
 * @brief evsip_mod_int_load
 * @param mod
 * @param info
 * @return
 */
unsigned int evsip_mod_int_load(evsip_mod_t *mod, evsip_mod_info_t *info);

/**
 * @brief evsip_mod_close
 * @param mod
 */
void evsip_mod_close(evsip_mod_t *mod);


#endif // _EVSIP_MOD_H_

// vim: noai:ts=2:sw=2
