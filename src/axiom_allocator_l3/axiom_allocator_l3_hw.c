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
#include <stdint.h>

#include "axiom_allocator.h"
#include "axiom_allocator_l3.h"
#include "dprintf.h"

static int
axiom_al3_hw_init(uintptr_t private_start, size_t private_size,
        uintptr_t shared_start, size_t shared_size)
{
    DPRINTF("TODO");
    return 0;
}

static int
axiom_al3_hw_addshreg(uintptr_t block_addr, size_t block_size)
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
    .init = axiom_al3_hw_init,
    .add_shregion = axiom_al3_hw_addshreg,
    .private_malloc = axiom_al3_hw_prmalloc,
    .shared_malloc = axiom_al3_hw_shmalloc,
    .private_free = axiom_al3_hw_prfree,
    .shared_free = axiom_al3_hw_shfree,
    .type = AXAL_HW,
    .overhead = 0,
};

static void axiom_al3hw_register(void) {
    axiom_al3_register(&axiom_al3hw_info);
}

module_init(axiom_al3hw_register);
