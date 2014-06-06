/*
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 */
#ifndef _ISIS_MISC_H_
#define _ISIS_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_misc.h"

sw_error_t isis_misc_init(a_uint32_t dev_id);

#ifdef IN_MISC
    #define ISIS_MISC_INIT(rv, dev_id) \
    { \
        rv = isis_misc_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
    #define ISIS_MISC_INIT(rv, dev_id)
#endif

#ifdef HSL_STANDALONG


HSL_LOCAL sw_error_t
isis_frame_max_size_set(a_uint32_t dev_id, a_uint32_t size);


HSL_LOCAL sw_error_t
isis_frame_max_size_get(a_uint32_t dev_id, a_uint32_t * size);


HSL_LOCAL sw_error_t
isis_port_unk_uc_filter_set(a_uint32_t dev_id, fal_port_t port_id,
                              a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_unk_uc_filter_get(a_uint32_t dev_id, fal_port_t port_id,
                              a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_unk_mc_filter_set(a_uint32_t dev_id, fal_port_t port_id,
                              a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_unk_mc_filter_get(a_uint32_t dev_id, fal_port_t port_id,
                              a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_bc_filter_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_bc_filter_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_cpu_port_status_set(a_uint32_t dev_id, a_bool_t enable);


HSL_LOCAL sw_error_t
isis_cpu_port_status_get(a_uint32_t dev_id, a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_pppoe_cmd_set(a_uint32_t dev_id, fal_fwd_cmd_t cmd);


HSL_LOCAL sw_error_t
isis_pppoe_cmd_get(a_uint32_t dev_id, fal_fwd_cmd_t * cmd);


HSL_LOCAL sw_error_t
isis_pppoe_status_set(a_uint32_t dev_id, a_bool_t enable);


HSL_LOCAL sw_error_t
isis_pppoe_status_get(a_uint32_t dev_id, a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_dhcp_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_dhcp_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_arp_cmd_set(a_uint32_t dev_id, fal_fwd_cmd_t cmd);


HSL_LOCAL sw_error_t
isis_arp_cmd_get(a_uint32_t dev_id, fal_fwd_cmd_t * cmd);


HSL_LOCAL sw_error_t
isis_eapol_cmd_set(a_uint32_t dev_id, fal_fwd_cmd_t cmd);


HSL_LOCAL sw_error_t
isis_eapol_cmd_get(a_uint32_t dev_id, fal_fwd_cmd_t * cmd);


HSL_LOCAL sw_error_t
isis_pppoe_session_table_add(a_uint32_t dev_id, fal_pppoe_session_t * session_tbl);


HSL_LOCAL sw_error_t
isis_pppoe_session_table_del(a_uint32_t dev_id, fal_pppoe_session_t * session_tbl);


HSL_LOCAL sw_error_t
isis_pppoe_session_table_get(a_uint32_t dev_id, fal_pppoe_session_t * session_tbl);

HSL_LOCAL sw_error_t
isis_eapol_status_set(a_uint32_t dev_id, a_uint32_t port_id, a_bool_t enable);

HSL_LOCAL sw_error_t
isis_eapol_status_get(a_uint32_t dev_id, a_uint32_t port_id, a_bool_t * enable);

HSL_LOCAL sw_error_t
isis_ripv1_status_set(a_uint32_t dev_id, a_bool_t enable);

HSL_LOCAL sw_error_t
isis_ripv1_status_get(a_uint32_t dev_id, a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_arp_req_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_arp_req_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t *enable);


HSL_LOCAL sw_error_t
isis_port_arp_ack_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_arp_ack_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t *enable);


#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif

