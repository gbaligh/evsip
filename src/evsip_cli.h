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

/**
 * @file evsip_cli.h
 * @brief
 */

#ifndef EVSIP_CLI_H_
#define EVSIP_CLI_H_

/** @brief Max command line length */
#define EVSIP_CLI_CMD_MAX_LEN	255

/**
 * Initialize the EvSip Command Line Interpreter
 *
 * @return @ref EVSIP_SUCCESS
 */
unsigned int evsip_cli_init(void);

/**
 * Start processing input from CLI
 *
 * @return
 * @ref EVSIP_SUCCESS
 * @ref EVSIP_ERROR_OUTOFRESOURCES
 */
unsigned int evsip_cli_start(void);

/**
 * Stop processing any input from CLI
 *
 * @return
 * @ref EVSIP_SUCCESS
 */
unsigned int evsip_cli_stop();

/**
 * Free all allocated memory used for CLI
 */
void evsip_cli_deinit();

#endif /* EVSIP_CLI_H_ */

//vim: noai:ts=2:sw=2
