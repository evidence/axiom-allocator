/*!
 * \file axiom_allocator.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the implementation of axiom-allocator API.
 */
#include <stdlib.h>
#include <stdio.h>

#include "axiom_allocator.h"
#include "axiom_allocator_l3/axiom_allocator_l3.h"
#include "dprintf.h"

static axiom_al3_info_t al3_info;

#define MAX_INFO_TABLE  2
static axiom_al3_info_t info_table[MAX_INFO_TABLE];
static int info_table_used = 0;

void
axiom_al3_register(axiom_al3_info_t *info)
{
    DPRINTF("registered new impl - type: %d", info->type);

    if (info_table_used < MAX_INFO_TABLE) {
        info_table[info_table_used++] = *info;
    }
}

int
axiom_allocator_init(size_t *private_size, size_t *shared_size,
        axiom_altype_t type)
{
    int i, ret = -1;

    for (i = 0; i < info_table_used; i++) {
        if (info_table[i].type == type) {
            ret = 0;
            break;
        }
    }

    if (ret != 0) {
        EPRINTF("type %d not found!", type);
        return ret;
    }

    al3_info = info_table[i];

    return al3_info.alloc_init(private_size, shared_size);
}

void *
axiom_private_malloc(size_t sz)
{
    return al3_info.private_malloc(sz);
}

void *
axiom_shared_malloc(size_t sz)
{
    return al3_info.shared_malloc(sz);
}

void
axiom_private_free(void *ptr)
{
    al3_info.private_free(ptr);
}

void
axiom_shared_free(void *ptr)
{
    al3_info.shared_free(ptr);
}
