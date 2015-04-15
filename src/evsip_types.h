/**
 * @file evsip_types.h
 * @brief Type and Const for EvSip
 */

#ifndef EVSIP_TYPES_H_
#define EVSIP_TYPES_H_

#define EVSIP_SUCCESS	0                      /**< EVSIP return when SUCCESS */

#define EVSIP_ERROR_UNKNOWN -1                 /**< There was an error, but we don't know specifics */
#define EVSIP_ERROR_OUTOFRESOURCES -2          /**< no resource left for this operation */
#define EVSIP_ERROR_BADPARAM -3                /**< parameter value invalid */
#define EVSIP_ERROR_NULLPTR -4                 /**< required pointer parameter was a NULL pointer */
#define EVSIP_ERROR_OUTOFRANGE -5              /**< parameter out of range */
#define EVSIP_ERROR_DESTRUCTED -6              /**< operation attempted on a destructed object */
#define EVSIP_ERROR_NOTSUPPORTED -7            /**< request not supported under current configuration */
#define EVSIP_ERROR_UNINITIALIZED -8           /**< object uninitialized */
#define EVSIP_ERROR_TRY_AGAIN -9               /**< incomplete operation, used by semaphore's try lock */
#define EVSIP_ERROR_ILLEGAL_ACTION -10         /**< the requested action is illegal */
#define EVSIP_ERROR_NETWORK_PROBLEM -11        /**< action failed due to network problems */
#define EVSIP_ERROR_INVALID_HANDLE -12         /**< a handle passed to a function is illegal */
#define EVSIP_ERROR_NOT_FOUND -13              /**< the requested item cannot be found */
#define EVSIP_ERROR_INSUFFICIENT_BUFFER -14    /**< the buffer is too small */

#ifdef __GNUC__
# define UNUSED(d) d __attribute__ ((unused))
#else
# define UNUSED(d) d
#endif

#define IN
#define OUT
#define INOUT

#define VALUE2STRING(_m)  #_m
#define MACRO2STRING(_m)  VALUE2STRING(_m)

#endif /* EVSIP_TYPES_H_ */

//vim: noai:ts=2:sw=2
