/*!
 * \file axiom_l2_allocator.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the implementation of axiom L2 allocator.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "axiom_l2_allocator.h"
#include "dprintf.h"

int
axiom_l2_init(uint8_t master_nodeid, uint8_t local_nodeid,
        uint64_t private_size, uint64_t shared_size)
{
    DPRINTF("TODO");
    return 0;
}

int
axiom_l2_get_prregion(uint64_t *start, uint64_t *size)
{
    DPRINTF("TODO");
    return 0;
}

int
axiom_l2_req_shregion(uint64_t *start, uint64_t *size)
{
    DPRINTF("TODO");
    return 0;
}
