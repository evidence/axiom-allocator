/*!
 * \file axiom_hw_l3_allocator.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the HW implementation of axiom L3 allocator.
 */
#include <stdlib.h>
#include <stdio.h>

#include "axiom_allocator.h"
#include "axiom_l3_allocator.h"
#include "dprintf.h"

static int
axiom_hw_l3_init(size_t shared_size, size_t private_size)
{
    DPRINTF("TODO");
    return 0;
}

static void *
axiom_hw_l3_prmalloc(size_t sz)
{
    DPRINTF("TODO");
    return NULL;
}

static void *
axiom_hw_l3_shmalloc(size_t sz)
{
    DPRINTF("TODO");
    return NULL;
}

static void
axiom_hw_l3_prfree(void *ptr)
{
    DPRINTF("TODO");
    return;
}

static void
axiom_hw_l3_shfree(void *ptr)
{
    DPRINTF("TODO");
    return;
}

static axiom_l3_info_t axiom_l3hw_info = {
    .alloc_init = axiom_hw_l3_init,
    .private_malloc = axiom_hw_l3_prmalloc,
    .shared_malloc = axiom_hw_l3_shmalloc,
    .private_free = axiom_hw_l3_prfree,
    .shared_free = axiom_hw_l3_shfree,
    .type = AXAL_HW,
};

static void axiom_l3hw_register(void) {
    axiom_l3_register(&axiom_l3hw_info);
}

module_init(axiom_l3hw_register);
