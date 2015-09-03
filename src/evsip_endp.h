/**
 * @file evsip_endp.h
 * @brief SIP end point controller private export
 *
 * @addtogroup Endp
 :q


 * SIP End Point
 * @{
 */

#ifndef EVSIP_ENDP_H_
#define EVSIP_ENDP_H_

/**
 * @brief End point Object
 */
typedef struct evsip_endp_ctx evsip_endp_t;

/**
 *
 * @param[in] pEndp
 * @return
 */
unsigned int evsip_endp_create(evsip_endp_t **pEndp);

/**
 *
 * @param[in] pEndp
 * @return
 */
unsigned int evsip_endp_add(evsip_endp_t *pEndp);

/**
 *
 * @param[in] pEndp
 * @return
 */
unsigned int evsip_endp_isAttached(evsip_endp_t *pEndp);

/**
 *
 * @param[in] pEndp
 * @return
 */
unsigned int evsip_endp_extract(evsip_endp_t *pEndp);

/**
 *
 * @param[in] pEndp
 * @param[in] pIpv4
 * @return
 */
unsigned int evsip_endp_setHostIpv4(evsip_endp_t *pEndp, const char *pIpv4);

/**
 *
 * @param[in] pEndp
 * @param[in] pContact
 * @return
 */
unsigned int evsip_endp_setContact(evsip_endp_t *pEndp, const char *pContact);

/**
 *
 * @param[in] pIpv4
 * @param[out] pEndp
 * @return @ref EVSIP_SUCCESS
 */
unsigned int evsip_endp_findByIpv4(const char *pIpv4, evsip_endp_t **pEndp);

/**
 * @brief Print all cached @ref evsip_endp_t "Endp".
 *
 * Write to console the cached list of endpoints
 */
void evsip_endp_dumpList();

/**
 *
 * @param[in] pEndp
 */
void evsip_endp_destroy(void *pEndp);

/** @} */
#endif /* EVSIP_ENDP_H_ */

//vim: noai:ts=2:sw=2
