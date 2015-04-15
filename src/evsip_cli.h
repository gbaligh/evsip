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
