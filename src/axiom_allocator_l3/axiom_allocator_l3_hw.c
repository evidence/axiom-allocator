/*!
 * \file axiom_allocator_l3_hw.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the HW implementation of axiom L3 allocator.
 */
#include <stdlib.h>
#include <stdio.h>

#include "axiom_allocator.h"
#include "axiom_allocator_l3.h"
#include "dprintf.h"

static int
axiom_al3_hw_init(size_t shared_size, size_t private_size)
{
    DPRINTF("TODO");
    return 0;
}

static void *
axiom_al3_hw_prmalloc(size_t sz)
{
    DPRINTF("TODO");
    return NULL;
}

static void *
axiom_al3_hw_shmalloc(size_t sz)
{
    DPRINTF("TODO");
    return NULL;
}

static void
axiom_al3_hw_prfree(void *ptr)
{
    DPRINTF("TODO");
    return;
}

static void
axiom_al3_hw_shfree(void *ptr)
{
    DPRINTF("TODO");
    return;
}

static axiom_al3_info_t axiom_al3hw_info = {
    .alloc_init = axiom_al3_hw_init,
    .private_malloc = axiom_al3_hw_prmalloc,
    .shared_malloc = axiom_al3_hw_shmalloc,
    .private_free = axiom_al3_hw_prfree,
    .shared_free = axiom_al3_hw_shfree,
    .type = AXAL_HW,
};

static void axiom_al3hw_register(void) {
    axiom_al3_register(&axiom_al3hw_info);
}

module_init(axiom_al3hw_register);
