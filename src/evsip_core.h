/*
 * evsip_core.h
 *
 *  Created on: 22 nov. 2014
 *      Author: G545485
 */

#ifndef EVSIP_CORE_H_
#define EVSIP_CORE_H_

#define EVSIP_UNUSED_ARG(x)	if((x)) {};

/**
 *
 * @return
 */
unsigned int evsip_init(void);

/**
 *
 * @return
 */
unsigned int evsip_start(void);

/**
 *
 * @return
 */
unsigned int evsip_stop(void);

/**
 *
 */
void evsip_deinit(void );

#endif /* EVSIP_CORE_H_ */
