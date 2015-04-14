/*
 * evsip_register.h
 *
 *  Created on: 19 déc. 2014
 *      Author: G545485
 */

#ifndef EVSIP_REGISTER_H_
#define EVSIP_REGISTER_H_

/**
 *
 * @param nua
 * @param nh
 * @param sip
 * @param tags
 * @return
 */
unsigned int evsip_register_handler(nua_t *nua, nua_handle_t *nh, sip_t const *sip, tagi_t tags[]);

#endif /* EVSIP_REGISTER_H_ */
