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

//vim: noai:ts=2:sw=2
