/*!
 * \file axiom_allocator_l1.c
 *
 * \version     v0.8
 * \date        2016-09-23
 *
 * This file contains the functions used in the axiom-init deamon to handle
 * the AXIOM L1 allocator messages.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "axiom_utility.h"
#include "axiom_nic_types.h"
#include "axiom_nic_packets.h"
#include "axiom_nic_raw_commands.h"
#include "axiom_nic_api_user.h"
#include "axiom_nic_init.h"
#include "dprintf.h"

#include "axiom_allocator_l1.h"
#include "axiom_allocator_l1_l2.h"

#include "evi_alloc.h"
#include "evi_queue.h"

/************************* Axiom Allocator L1 CORE ****************************/
typedef struct axiom_al1core {
    uintptr_t memory_start;
    size_t memory_size;
    size_t block_size;
    evi_alloc_t alloc_table;
    evi_queue_t app_id;
} axiom_al1core_t;

static void
axiom_al1core_init(axiom_al1core_t *l1)
{
    l1->memory_size = AXIOM_ALLOCATOR_MEM_SIZE;
    l1->memory_start = AXIOM_ALLOCATOR_MEM_START;
    l1->block_size = AXIOM_ALLOCATOR_L1_BSIZE;
    evia_init(&l1->alloc_table,
            l1->memory_size / l1->block_size);
    eviq_init(&l1->app_id, 0, AXIOM_ALLOCATOR_NUM_APP_ID);
}

static int
axiom_al1core_alloc(axiom_al1core_t *l1, uintptr_t *addr, size_t *size,
        axiom_app_id_t app_id)
{
    int num_blocks, start;

    num_blocks = *size / l1->block_size;
    if ((num_blocks * l1->block_size) < *size)
        num_blocks++;

    *size = num_blocks * l1->block_size;

    start = evia_alloc(&l1->alloc_table, app_id, num_blocks);
    if (start < 0)
        return start;

    *addr = (((uintptr_t)start) * l1->block_size) + l1->memory_start;

    return 0;
}

static axiom_app_id_t
axiom_al1core_alloc_appid(axiom_al1core_t *l1)
{
    axiom_app_id_t app_id;

    /* alloc a new application ID */
    app_id = eviq_free_pop(&l1->app_id);
    if (app_id == EVIQ_NONE) {
        return AXIOM_NULL_APP_ID;
    }

    return app_id;
}

static void
axiom_al1core_release(axiom_al1core_t *l1, axiom_app_id_t app_id)
{
    /* release all blocks owned by app_id */
    evia_free(&l1->alloc_table, app_id);
    /* release app_id */
    eviq_free_push(&l1->app_id, app_id);
}


/*********************** Axiom Allocator L1 PROTCOL ***************************/
static axiom_al1core_t al1_core;

void
axiom_al1_init(void)
{
    axiom_al1core_init(&al1_core);
}

int
axiom_al1_alloc(axiom_alloc_msg_t *info)
{
    int ret;

    if (info->app_id == AXIOM_NULL_APP_ID)
        goto err;

    /* shared blocks */
    if (info->shared_size > 0) {
        ret = axiom_al1core_alloc(&al1_core, &(info->shared_start),
                &(info->shared_size), info->app_id);

        if (ret < 0)
            goto err;
    }

    /* private blocks */
    if (info->private_size > 0) {
        ret = axiom_al1core_alloc(&al1_core, &(info->private_start),
                &(info->private_size), info->app_id);

        if (ret < 0)
            goto err;
    }

    info->error = AXIOM_RET_OK;
    return 1;

err:
    info->error = AXIOM_RET_ERROR;
    return 1;
}

int
axiom_al1_alloc_appid(axiom_alloc_msg_t *info)
{
    info->app_id = axiom_al1core_alloc_appid(&al1_core);
    if (info->app_id == AXIOM_NULL_APP_ID)
        info->error = AXIOM_RET_ERROR;
    else
        info->error = AXIOM_RET_OK;

    return 1;
}

int
axiom_al1_release(axiom_alloc_msg_t *info)
{
    axiom_al1core_release(&al1_core, info->app_id);
    return 0;
}

