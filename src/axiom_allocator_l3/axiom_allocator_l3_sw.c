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

static int
axiom_al3_sw_init(size_t shared_size, size_t private_size)
{
    return axiom_al23_init(private_size, shared_size);
}

static void *
axiom_al3_sw_prmalloc(size_t sz)
{
    DPRINTF("TODO");
    return NULL;
}

static void *
axiom_al3_sw_shmalloc(size_t sz)
{
    DPRINTF("TODO");
    return NULL;
}

static void
axiom_al3_sw_prfree(void *ptr)
{
    DPRINTF("TODO");
    return;
}

static void
axiom_al3_sw_shfree(void *ptr)
{
    DPRINTF("TODO");
    return;
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
