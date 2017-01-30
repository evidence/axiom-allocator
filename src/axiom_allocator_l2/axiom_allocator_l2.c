/*!
 * \file axiom_allocator_l2.c
 *
 * \version     v0.11
 * \date        2016-10-13
 *
 * This file contains the AXIOM allocator level 2.
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "dprintf.h"
#include "evi_alloc.h"
#include "axiom_nic_limits.h"
#include "axiom_init_api.h"
#include "axiom_allocator_protocol.h"
#include "axiom_allocator_l2.h"
#include "axiom_allocator_l1_l2.h"
#include "axiom_allocator_l2_l3.h"


/************************* Axiom Allocator L2 CORE ****************************/

typedef struct axiom_al2core {
    evi_alloc_t shared_alloc_table;
    uintptr_t shared_start;
    size_t shared_size;
    size_t shared_block_size;
} axiom_al2core_t;

static void
axiom_al2core_init(axiom_al2core_t *l2, uintptr_t shared_start,
        size_t shared_size)
{
    l2->shared_start = shared_start;
    l2->shared_size = shared_size;
    l2->shared_block_size = AXIOM_ALLOCATOR_L2_BSIZE;
    evia_init(&l2->shared_alloc_table,
            l2->shared_size / l2->shared_block_size);
}

static int
axiom_al2core_alloc(axiom_al2core_t *l2, uintptr_t *addr, size_t *size,
        axiom_node_id_t node_id)
{
    int num_blocks, start;

    num_blocks = *size / l2->shared_block_size;
    if ((num_blocks * l2->shared_block_size) < *size)
        num_blocks++;

    *size = num_blocks * l2->shared_block_size;

    start = evia_alloc(&l2->shared_alloc_table, node_id, num_blocks);
    if (start < 0)
        return start;

    *addr = (((uintptr_t)start) * l2->shared_block_size) + l2->shared_start;

    return 0;
}

static void
axiom_al2core_free(axiom_al2core_t *l2, axiom_node_id_t node_id)
{
    evia_free(&l2->shared_alloc_table, node_id);
}

static void
axiom_al2core_release(axiom_al2core_t *l2)
{
    evia_release(&l2->shared_alloc_table);
}

/*********************** Axiom Allocator L2 PROTOCOL **************************/

typedef struct axiom_al2proto {
    enum {AXL2ST_INIT = 1, AXL2ST_ALLOC, AXL2ST_SETUP, AXL2ST_RELEASE} status;
    axiom_app_id_t app_id;
    uintptr_t private_start;
    size_t private_size;
    uintptr_t shared_start;
    size_t shared_size;
    axiom_al2core_t al2_core;
} axiom_al2proto_t;

static axiom_al2proto_t al2_proto = {
    .status = AXL2ST_RELEASE,
};

int
axiom_al2_init(axiom_app_id_t app_id)
{
    if (al2_proto.status != AXL2ST_RELEASE) {
        EPRINTF("MASTER: allocator status [%d] unexpected", al2_proto.status);
        return -1;
    }

    if (app_id != AXIOM_NULL_APP_ID) {
        al2_proto.app_id = app_id;
        al2_proto.status = AXL2ST_INIT;
    }

    return 0;
}

void
axiom_al2_release(axiom_dev_t *dev)
{
    axiom_err_t ret;

    if (al2_proto.status != AXL2ST_RELEASE) {
        ret = axiom_al12_release(dev, al2_proto.app_id);
        if (!AXIOM_RET_IS_OK(ret)) {
            EPRINTF("MASTER: axiom_al12_release() error %d", ret);
        }

        axiom_al2core_release(&al2_proto.al2_core);

        al2_proto.status = AXL2ST_RELEASE;
    }
}

int
axiom_al2_alloc_reply(axiom_dev_t *dev, size_t size, void *inmsg,
        axiom_alloc_msg_t *info)
{
    axiom_err_t ret;

    info->error = AXIOM_RET_OK;

    if (al2_proto.status != AXL2ST_ALLOC) {
        EPRINTF("MASTER: allocator status [%d] unexpected", al2_proto.status);
        info->error = AXIOM_RET_ERROR;
        goto reply;
    }

    ret = axiom_al12_alloc_parsereply(inmsg, size, &info->private_start,
            &info->private_size, &info->shared_start,
            &info->shared_size);
    if (!AXIOM_RET_IS_OK(ret)) {
        EPRINTF("MASTER: axiom_al12_alloc_parsereply() error %d", ret);
        info->error = ret;
        goto reply;
    }

    /* setup the allocator */
    axiom_al2core_init(&al2_proto.al2_core, info->shared_start,
            info->shared_size);

    al2_proto.private_start = info->private_start;
    al2_proto.private_size = info->private_size;
    al2_proto.shared_start = info->shared_start;
    al2_proto.shared_size = info->shared_size;

    al2_proto.status = AXL2ST_SETUP;

reply:
    return 1;
}

int
axiom_al2_alloc(axiom_dev_t *dev, axiom_port_t master_port, size_t size,
        void *buffer)
{
    axiom_alloc_msg_t *info = (axiom_alloc_msg_t *)buffer;
    axiom_err_t ret;

    info->error = AXIOM_RET_OK;

    if (al2_proto.status != AXL2ST_INIT) {
        EPRINTF("MASTER: allocator status [%d] unexpected", al2_proto.status);
        info->error = AXIOM_RET_ERROR;
        goto reply;
    }

    al2_proto.status = AXL2ST_ALLOC;

    ret = axiom_al12_alloc(dev, master_port, al2_proto.app_id, info->private_size,
            info->shared_size);
    if (!AXIOM_RET_IS_OK(ret)) {
        EPRINTF("MASTER: axiom_al12_alloc error %d", ret);
        al2_proto.status = AXL2ST_INIT;
        info->error = AXIOM_RET_ERROR;
        goto reply;
    }

    /* not send reply, we need a reply from master init */
    return 0;

reply:
    return 1;
}

int
axiom_al2_alloc_appid(axiom_dev_t *dev, axiom_port_t master_port,
        axiom_app_id_t *app_id)
{
    axiom_err_t err;

    /* send request for unique app-id */
    err = axiom_al12_get_appid(dev, master_port);
    if (!AXIOM_RET_IS_OK(err)) {
        EPRINTF("axal12_get_appid() ret: %d", err);
        return -1;
    }

    /* wait reply of unique app-id */
    err = axiom_al12_get_appid_reply(dev, app_id);
    if (!AXIOM_RET_IS_OK(err)) {
        EPRINTF("axal12_get_appid_reply() ret: %d", err);
        return -1;
    }

    return 0;
}

int
axiom_al2_get_regions(axiom_dev_t *dev, axiom_node_id_t src_node,
        size_t size, void *buffer)
{
    axiom_alloc_msg_t *info = (axiom_alloc_msg_t *)buffer;

    info->error = AXIOM_RET_OK;

    if (al2_proto.status != AXL2ST_SETUP) {
        EPRINTF("MASTER: allocator status [%d] unexpected", al2_proto.status);
        info->error = AXIOM_RET_ERROR;
        goto reply;
    }

    info->private_start = al2_proto.private_start;
    info->private_size = al2_proto.private_size;
    info->shared_start = al2_proto.shared_start;
    info->shared_size = al2_proto.shared_size;

reply:
    return 1;
}

int
axiom_al2_alloc_shblock(axiom_dev_t *dev, axiom_node_id_t src_node,
        size_t size, void *buffer)
{
    axiom_alloc_msg_t *info = (axiom_alloc_msg_t *)buffer;
    axiom_err_t ret;

    info->error = AXIOM_RET_OK;

    if (al2_proto.status != AXL2ST_SETUP) {
        EPRINTF("MASTER: allocator status [%d] unexpected", al2_proto.status);
        info->error = AXIOM_RET_ERROR;
        goto reply;
    }

    /* alloc new blocks TODO: handle no memory error */
    ret = axiom_al2core_alloc(&al2_proto.al2_core, &(info->shared_start),
            &(info->shared_size), src_node);
    if (ret < 0) {
        EPRINTF("MASTER: axiom_al2core_alloc error %d", ret);
        info->error = AXIOM_RET_NOMEM;
        goto reply;
    }

reply:
    return 1;
}
