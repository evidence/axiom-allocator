/*!
 * \file axiom_allocator_l3_sw.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the SW implementation of axiom L3 allocator.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "axiom_allocator.h"
#include "axiom_allocator_l3.h"
#include "axiom_allocator_l2_l3.h"
#include "dprintf.h"
#include "evi_allocator.h"

static int
axiom_al3_sw_init(size_t shared_size, size_t private_size)
{
    uintptr_t private_start, shared_start;
    int ret;

    /* init L2 allocator */
    ret = axiom_al23_init(private_size, shared_size);
    if (ret) {
        return AXAL_RET_ERROR;
    }

    /* take private and shared region from L2 allocator */
    ret = axiom_al23_get_regions(&private_start, &private_size, &shared_start,
            &shared_size);
    if (ret) {
        return AXAL_RET_ERROR;
    }

    /* init L3 allocator */
    ret = evi_allocator_init(shared_start, shared_start + shared_size,
            private_start, private_start + private_size);
    if (ret) {
        return AXAL_RET_ERROR;
    }

    return AXAL_RET_OK;
}

static void *
axiom_al3_sw_prmalloc(size_t sz)
{
    return evi_private_malloc(sz);
}

static void *
axiom_al3_sw_shmalloc(size_t sz)
{
    int ret;
    void *addr;
    uintptr_t block_addr;
    size_t block_size;

    addr = evi_shared_malloc(sz);
    if (addr) {
        return addr;
    }

    /* if L3 malloc fails, request a new shared block from L2 allocator */
    block_size = AXIOM_ALLOCATOR_L2_BSIZE;
    ret = axiom_al23_alloc_shblock(&block_addr, &block_size);
    if (ret) {
        return NULL;
    }

    /* add the new block to L3 allocator */
    ret = evi_add_shared_region(block_addr, block_size);
    if (ret) {
        return NULL;
    }

    /* retry L3 allocation */
    return evi_shared_malloc(sz);
}

static void
axiom_al3_sw_prfree(void *ptr)
{
    evi_private_free(ptr);
}

static void
axiom_al3_sw_shfree(void *ptr)
{
    evi_shared_free(ptr);
}

static axiom_al3_info_t axiom_al3sw_info = {
    .alloc_init = axiom_al3_sw_init,
    .private_malloc = axiom_al3_sw_prmalloc,
    .shared_malloc = axiom_al3_sw_shmalloc,
    .private_free = axiom_al3_sw_prfree,
    .shared_free = axiom_al3_sw_shfree,
    .type = AXAL_SW,
};

static void axiom_al3sw_register(void) {
    axiom_al3_register(&axiom_al3sw_info);
}

module_init(axiom_al3sw_register);
