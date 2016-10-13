/*!
 * \file axiom_allocator_l2_l3.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the implementation of AXIOM L2 L3 allocator interface.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../axiom_allocator_l2_l3.h"
#include "dprintf.h"

int
axiom_al23_init(uint8_t master_nodeid, uint8_t local_nodeid,
        uint64_t private_size, uint64_t shared_size)
{
    DPRINTF("TODO");
    return 0;
}

int
axiom_al23_get_prregion(uint64_t *start, uint64_t *size)
{
    DPRINTF("TODO");
    return 0;
}

int
axiom_al23_req_shregion(uint64_t *start, uint64_t *size)
{
    DPRINTF("TODO");
    return 0;
}
