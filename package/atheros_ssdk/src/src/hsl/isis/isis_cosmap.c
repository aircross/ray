/*
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 */
/**
 * @defgroup isis_cosmap ISIS_COSMAP
 * @{
 */
#include "sw.h"
#include "hsl.h"
#include "hsl_dev.h"
#include "hsl_port_prop.h"
#include "isis_cosmap.h"
#include "isis_reg.h"

#define ISIS_MAX_DSCP                63
#define ISIS_MAX_UP                  7
#define ISIS_MAX_PRI                 7
#define ISIS_MAX_DP                  1
#define ISIS_MAX_QUEUE               3
#define ISIS_MAX_EH_QUEUE            5

#define ISIS_DSCP_TO_PRI             0
#define ISIS_DSCP_TO_DP              1
#define ISIS_UP_TO_PRI               2
#define ISIS_UP_TO_DP                3

#define ISIS_EGRESS_REAMRK_ADDR      0x5ae00
#define ISIS_EGRESS_REAMRK_NUM       16

static sw_error_t
_isis_cosmap_dscp_to_pri_dp_set(a_uint32_t dev_id, a_uint32_t mode,
                                a_uint32_t dscp, a_uint32_t val)
{
    sw_error_t rv;
    a_uint32_t index, data;

    if (ISIS_MAX_DSCP < dscp) {
        return SW_BAD_PARAM;
    }

    index = dscp >> 3;
    HSL_REG_ENTRY_GET(rv, dev_id, DSCP_TO_PRI, index, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    if (ISIS_DSCP_TO_PRI == mode) {
        if (ISIS_MAX_PRI < val) {
            return SW_BAD_PARAM;
        }

        data &= (~(0x7 << ((dscp & 0x7) << 2)));
        data |= (val << ((dscp & 0x7) << 2));
    } else {
        if (ISIS_MAX_DP < val) {
            return SW_BAD_PARAM;
        }

        data &= (~(0x1 << (((dscp & 0x7) << 2) + 3)));
        data |= (val << (((dscp & 0x7) << 2) + 3));
    }

    HSL_REG_ENTRY_SET(rv, dev_id, DSCP_TO_PRI, index, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_isis_cosmap_dscp_to_pri_dp_get(a_uint32_t dev_id, a_uint32_t mode,
                                a_uint32_t dscp, a_uint32_t * val)
{
    sw_error_t rv;
    a_uint32_t index, data;

    if (ISIS_MAX_DSCP < dscp) {
        return SW_BAD_PARAM;
    }

    index = dscp >> 3;
    HSL_REG_ENTRY_GET(rv, dev_id, DSCP_TO_PRI, index, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    data = (data >> ((dscp & 0x7) << 2)) & 0xf;
    if (ISIS_DSCP_TO_PRI == mode) {
        *val = data & 0x7;
    } else {
        *val = (data & 0x8) >> 3;
    }

    return SW_OK;
}

static sw_error_t
_isis_cosmap_up_to_pri_dp_set(a_uint32_t dev_id, a_uint32_t mode, a_uint32_t up,
                              a_uint32_t val)
{
    sw_error_t rv;
    a_uint32_t data;

    if (ISIS_MAX_UP < up) {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, UP_TO_PRI, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    if (ISIS_UP_TO_PRI == mode) {
        if (ISIS_MAX_PRI < val) {
            return SW_BAD_PARAM;
        }

        data &= (~(0x7 << (up << 2)));
        data |= (val << (up << 2));
    } else {
        if (ISIS_MAX_DP < val) {
            return SW_BAD_PARAM;
        }

        data &= (~(0x1 << ((up << 2) + 3)));
        data |= (val << ((up << 2) + 3));
    }

    HSL_REG_ENTRY_SET(rv, dev_id, UP_TO_PRI, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_isis_cosmap_up_to_pri_dp_get(a_uint32_t dev_id, a_uint32_t mode, a_uint32_t up,
                              a_uint32_t * val)
{
    sw_error_t rv;
    a_uint32_t data;

    if (ISIS_MAX_UP < up) {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, UP_TO_PRI, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    data = (data >> (up << 2)) & 0xf;

    if (ISIS_UP_TO_PRI == mode) {
        *val = (data & 0x7);
    } else {
        *val = (data & 0x8) >> 3;
    }

    return SW_OK;
}

static sw_error_t
_isis_cosmap_pri_to_queue_set(a_uint32_t dev_id, a_uint32_t pri,
                              a_uint32_t queue)
{
    sw_error_t rv;
    a_uint32_t data;

    if ((ISIS_MAX_PRI < pri) || (ISIS_MAX_QUEUE < queue)) {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, PRI_TO_QUEUE, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    data &= (~(0x3 << (pri << 2)));
    data |= (queue << (pri << 2));

    HSL_REG_ENTRY_SET(rv, dev_id, PRI_TO_QUEUE, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_isis_cosmap_pri_to_queue_get(a_uint32_t dev_id, a_uint32_t pri,
                              a_uint32_t * queue)
{
    sw_error_t rv;
    a_uint32_t data;

    if (ISIS_MAX_PRI < pri) {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, PRI_TO_QUEUE, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    *queue = (data >> (pri << 2)) & 0x3;
    return SW_OK;
}

static sw_error_t
_isis_cosmap_pri_to_ehqueue_set(a_uint32_t dev_id, a_uint32_t pri,
                                a_uint32_t queue)
{
    sw_error_t rv;
    a_uint32_t data;

    if ((ISIS_MAX_PRI < pri) || (ISIS_MAX_EH_QUEUE < queue)) {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, PRI_TO_EHQUEUE, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    data &= (~(0x7 << (pri << 2)));
    data |= (queue << (pri << 2));

    HSL_REG_ENTRY_SET(rv, dev_id, PRI_TO_EHQUEUE, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_isis_cosmap_pri_to_ehqueue_get(a_uint32_t dev_id, a_uint32_t pri,
                                a_uint32_t * queue)
{
    sw_error_t rv;
    a_uint32_t data;

    if (ISIS_MAX_PRI < pri) {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, PRI_TO_EHQUEUE, 0, (a_uint8_t *) (&data),
                      sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    *queue = (data >> (pri << 2)) & 0x7;
    return SW_OK;
}

static sw_error_t
_isis_cosmap_egress_remark_set(a_uint32_t dev_id, a_uint32_t tbl_id,
                               fal_egress_remark_table_t * tbl)
{
    sw_error_t rv;
    a_uint32_t data, addr;

    if (ISIS_EGRESS_REAMRK_NUM <= tbl_id) {
        return SW_BAD_PARAM;
    }

    data = (tbl->y_up & 0x7)
        | ((tbl->g_up & 0x7) << 4)
        | ((tbl->y_dscp & 0x3f) << 8)
        | ((tbl->g_dscp & 0x3f) << 16)
        | ((tbl->remark_dscp & 0x1) << 23)
        | ((tbl->remark_up & 0x1) << 22);

    addr = ISIS_EGRESS_REAMRK_ADDR + (tbl_id << 2);
    HSL_REG_ENTRY_GEN_SET(rv, dev_id, addr, sizeof (a_uint32_t),
                          (a_uint8_t *) (&data), sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_isis_cosmap_egress_remark_get(a_uint32_t dev_id, a_uint32_t tbl_id,
                               fal_egress_remark_table_t * tbl)
{
    sw_error_t rv;
    a_uint32_t data, addr;

    if (ISIS_EGRESS_REAMRK_NUM <= tbl_id) {
        return SW_BAD_PARAM;
    }

    aos_mem_zero(tbl, sizeof (fal_egress_remark_table_t));

    addr = ISIS_EGRESS_REAMRK_ADDR + (tbl_id << 2);
    HSL_REG_ENTRY_GEN_GET(rv, dev_id, addr, sizeof (a_uint32_t),
                          (a_uint8_t *) (&data), sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    if (data & (0x1 << 23)) {
        tbl->remark_dscp = A_TRUE;
        tbl->y_dscp = (data >> 8) & 0x3f;
        tbl->g_dscp = (data >> 16) & 0x3f;
    }

    if (data & (0x1 << 22)) {
        tbl->remark_up = A_TRUE;
        tbl->y_up = data & 0x7;
        tbl->g_up = (data >> 4) & 0x7;
    }

    return SW_OK;
}

/**
 * @brief Set dscp to internal priority mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] dscp dscp
 * @param[in] pri internal priority 
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_dscp_to_pri_set(a_uint32_t dev_id, a_uint32_t dscp, a_uint32_t pri)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_dscp_to_pri_dp_set(dev_id, ISIS_DSCP_TO_PRI, dscp, pri);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get dscp to internal priority mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] dscp dscp
 * @param[out] pri internal priority 
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_dscp_to_pri_get(a_uint32_t dev_id, a_uint32_t dscp,
                            a_uint32_t * pri)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_dscp_to_pri_dp_get(dev_id, ISIS_DSCP_TO_PRI, dscp, pri);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set dscp to internal drop precedence mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] dscp dscp
 * @param[in] dp internal drop precedence
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_dscp_to_dp_set(a_uint32_t dev_id, a_uint32_t dscp, a_uint32_t dp)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_dscp_to_pri_dp_set(dev_id, ISIS_DSCP_TO_DP, dscp, dp);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get dscp to internal drop precedence mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] dscp dscp
 * @param[out] dp internal drop precedence
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_dscp_to_dp_get(a_uint32_t dev_id, a_uint32_t dscp, a_uint32_t * dp)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_dscp_to_pri_dp_get(dev_id, ISIS_DSCP_TO_DP, dscp, dp);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set dot1p to internal priority mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] up dot1p
 * @param[in] pri internal priority 
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_up_to_pri_set(a_uint32_t dev_id, a_uint32_t up, a_uint32_t pri)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_up_to_pri_dp_set(dev_id, ISIS_UP_TO_PRI, up, pri);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get dot1p to internal priority mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] up dot1p
 * @param[out] pri internal priority 
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_up_to_pri_get(a_uint32_t dev_id, a_uint32_t up, a_uint32_t * pri)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_up_to_pri_dp_get(dev_id, ISIS_UP_TO_PRI, up, pri);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set dot1p to internal drop precedence mapping on one particular device.
 * @param[in] dev_id device id
 * @param[in] up dot1p
 * @param[in] dp internal drop precedence
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_up_to_dp_set(a_uint32_t dev_id, a_uint32_t up, a_uint32_t dp)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_up_to_pri_dp_set(dev_id, ISIS_UP_TO_DP, up, dp);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get dot1p to internal drop precedence mapping on one particular device.  
 * @param[in] dev_id device id
 * @param[in] up dot1p
 * @param[in] dp internal drop precedence
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_up_to_dp_get(a_uint32_t dev_id, a_uint32_t up, a_uint32_t * dp)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_up_to_pri_dp_get(dev_id, ISIS_UP_TO_DP, up, dp);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set internal priority to queue mapping on one particular device.
 * @details  Comments:
 * This function is for port 1/2/3/4 which have four egress queues
 * @param[in] dev_id device id
 * @param[in] pri internal priority
 * @param[in] queue queue id
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_pri_to_queue_set(a_uint32_t dev_id, a_uint32_t pri,
                             a_uint32_t queue)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_pri_to_queue_set(dev_id, pri, queue);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get internal priority to queue mapping on one particular device.
 * @details  Comments:
 * This function is for port 1/2/3/4 which have four egress queues
 * @param[in] dev_id device id
 * @param[in] pri internal priority
 * @param[out] queue queue id
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_pri_to_queue_get(a_uint32_t dev_id, a_uint32_t pri,
                             a_uint32_t * queue)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_pri_to_queue_get(dev_id, pri, queue);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set internal priority to queue mapping on one particular device.
 * @details  Comments:
 * This function is for port 0/5/6 which have six egress queues
 * @param[in] dev_id device id
 * @param[in] pri internal priority
 * @param[in] queue queue id
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_pri_to_ehqueue_set(a_uint32_t dev_id, a_uint32_t pri,
                               a_uint32_t queue)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_pri_to_ehqueue_set(dev_id, pri, queue);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get internal priority to queue mapping on one particular device.
 * @details  Comments:
 * This function is for port 0/5/6 which have six egress queues
 * @param[in] dev_id device id
 * @param[in] pri internal priority
 * @param[in] queue queue id
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_pri_to_ehqueue_get(a_uint32_t dev_id, a_uint32_t pri,
                               a_uint32_t * queue)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_pri_to_ehqueue_get(dev_id, pri, queue);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set egress queue based CoS remap table on one particular device.
 * @param[in] dev_id device id
 * @param[in] tbl_id CoS remap table id
 * @param[in] tbl CoS remap table
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_egress_remark_set(a_uint32_t dev_id, a_uint32_t tbl_id,
                              fal_egress_remark_table_t * tbl)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_egress_remark_set(dev_id, tbl_id, tbl);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get egress queue based CoS remap table on one particular device.
 * @param[in] dev_id device id
 * @param[in] tbl_id CoS remap table id
 * @param[out] tbl CoS remap table
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
isis_cosmap_egress_remark_get(a_uint32_t dev_id, a_uint32_t tbl_id,
                              fal_egress_remark_table_t * tbl)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _isis_cosmap_egress_remark_get(dev_id, tbl_id, tbl);
    HSL_API_UNLOCK;
    return rv;
}

sw_error_t
isis_cosmap_init(a_uint32_t dev_id)
{
    HSL_DEV_ID_CHECK(dev_id);

#ifndef HSL_STANDALONG
    {
        hsl_api_t *p_api;

        SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

        p_api->cosmap_dscp_to_pri_set = isis_cosmap_dscp_to_pri_set;
        p_api->cosmap_dscp_to_pri_get = isis_cosmap_dscp_to_pri_get;
        p_api->cosmap_dscp_to_dp_set = isis_cosmap_dscp_to_dp_set;
        p_api->cosmap_dscp_to_dp_get = isis_cosmap_dscp_to_dp_get;
        p_api->cosmap_up_to_pri_set = isis_cosmap_up_to_pri_set;
        p_api->cosmap_up_to_pri_get = isis_cosmap_up_to_pri_get;
        p_api->cosmap_up_to_dp_set = isis_cosmap_up_to_dp_set;
        p_api->cosmap_up_to_dp_get = isis_cosmap_up_to_dp_get;
        p_api->cosmap_pri_to_queue_set = isis_cosmap_pri_to_queue_set;
        p_api->cosmap_pri_to_queue_get = isis_cosmap_pri_to_queue_get;
        p_api->cosmap_pri_to_ehqueue_set = isis_cosmap_pri_to_ehqueue_set;
        p_api->cosmap_pri_to_ehqueue_get = isis_cosmap_pri_to_ehqueue_get;
        p_api->cosmap_egress_remark_set = isis_cosmap_egress_remark_set;
        p_api->cosmap_egress_remark_get = isis_cosmap_egress_remark_get;
    }
#endif

    return SW_OK;
}

/**
 * @}
 */

