/*!
 * \file axiom_allocator_l1_l2.c
 *
 * \version     v1.2
 * \date        2016-09-23
 *
 * This file contains the interface between AXIOM L1 and L2 allocator.
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "axiom_nic_raw_commands.h"
#include "axiom_nic_limits.h"
#include "axiom_nic_packets.h"
#include "axiom_nic_api_user.h"
#include "axiom_nic_init.h"
#include "axiom_allocator_l1_l2.h"


axiom_err_t
axiom_al12_get_appid(axiom_dev_t *dev, axiom_port_t reply_port)
{
    axiom_allocator_payload_t alloc_payload;
    axiom_err_t ret;

    alloc_payload.command = AXIOM_CMD_ALLOC_APPID;
    alloc_payload.reply_port = reply_port;

    ret = axiom_send_raw(dev, AXIOM_INIT_MASTER_NODE, AXIOM_RAW_PORT_INIT,
            AXIOM_TYPE_RAW_DATA, sizeof(alloc_payload), &alloc_payload);
    if (!AXIOM_RET_IS_OK(ret)) {
        return ret;
    }

    return AXIOM_RET_OK;
}

axiom_err_t
axiom_al12_get_appid_reply(axiom_dev_t *dev, axiom_app_id_t *appid)
{
    axiom_allocator_payload_t payload;
    axiom_raw_payload_size_t size = sizeof(payload);
    axiom_node_id_t src;
    axiom_port_t port;
    axiom_type_t type;
    axiom_err_t ret;

    ret = axiom_recv_raw(dev, &src, &port, &type, &size, &payload);
    if (!AXIOM_RET_IS_OK(ret)) {
        EPRINTF("error: %d", ret);
        return ret;
    }

    if ((src != AXIOM_INIT_MASTER_NODE) ||
            (payload.command != AXIOM_CMD_ALLOC_APPID_REPLY) ||
            (payload.info.error != AXIOM_RET_OK)) {
        EPRINTF("info.error: %d", payload.info.error);
        return AXIOM_RET_ERROR;
    }

    *appid = payload.info.app_id;

    return AXIOM_RET_OK;
}

axiom_err_t
axiom_al12_alloc(axiom_dev_t *dev, axiom_port_t reply_port, axiom_app_id_t appid,
        size_t private_size, size_t shared_size)
{
    axiom_allocator_payload_t alloc_payload;
    axiom_err_t ret;

    alloc_payload.command = AXIOM_CMD_ALLOC;
    alloc_payload.reply_port = reply_port;
    alloc_payload.info.app_id = appid;
    alloc_payload.info.private_size = private_size;
    alloc_payload.info.shared_size = shared_size;

    ret = axiom_send_raw(dev, AXIOM_INIT_MASTER_NODE, AXIOM_RAW_PORT_INIT,
            AXIOM_TYPE_RAW_DATA, sizeof(alloc_payload), &alloc_payload);
    if (!AXIOM_RET_IS_OK(ret)) {
        return ret;
    }

    return AXIOM_RET_OK;
}

axiom_err_t
axiom_al12_alloc_parsereply(void *payload, size_t payload_size,
        uintptr_t *private_start, size_t *private_size, uintptr_t *shared_start,
        size_t *shared_size)
{
    axiom_allocator_payload_t *alloc_payload =
        ((axiom_allocator_payload_t *) payload);

    if (payload_size != sizeof(*alloc_payload) ||
            !AXIOM_RET_IS_OK(alloc_payload->info.error) ||
            alloc_payload->command != AXIOM_CMD_ALLOC_REPLY) {
        return AXIOM_RET_NOMEM;
    }

    *private_start = alloc_payload->info.private_start;
    *private_size = alloc_payload->info.private_size;
    *shared_start = alloc_payload->info.shared_start;
    *shared_size = alloc_payload->info.shared_size;

    return AXIOM_RET_OK;
}

axiom_err_t
axiom_al12_release(axiom_dev_t *dev, axiom_app_id_t appid)
{
    axiom_allocator_payload_t alloc_payload;
    axiom_err_t ret;

    alloc_payload.command = AXIOM_CMD_ALLOC_RELEASE;
    alloc_payload.info.app_id = appid;

    ret = axiom_send_raw(dev, AXIOM_INIT_MASTER_NODE, AXIOM_RAW_PORT_INIT,
            AXIOM_TYPE_RAW_DATA, sizeof(alloc_payload), &alloc_payload);
    if (!AXIOM_RET_IS_OK(ret)) {
        return ret;
    }

    return AXIOM_RET_OK;
}
