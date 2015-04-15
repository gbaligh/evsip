#ifndef EVSIP_AL_H_
#define EVSIP_AL_H_

/**
 *
 * @param event
 * @param status
 * @param phrase
 * @param nua
 * @param magic
 * @param nh
 * @param hmagic
 * @param sip
 * @param tags
 */
void evsip_evsofia_main(nua_event_t   event,
    int           status,
    char const   *phrase,
    nua_t        *nua,
    nua_magic_t  *magic,
    nua_handle_t *nh,
    nua_hmagic_t *hmagic,
    sip_t const  *sip,
    tagi_t        tags[]);

#endif /* EVSIP_HSOFIA_H_ */

//vim: noai:ts=2:sw=2
