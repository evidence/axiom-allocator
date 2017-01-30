/*!
 * \file axiom_allocator_l3_sw.c
 *
 * \version     v0.10
 * \date        2016-09-29
 *
 * This file contains the SW implementation of axiom L3 allocator.
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "axiom_allocator.h"
#include "axiom_allocator_l3.h"
#include "dprintf.h"
#include "evi_lmm.h"
#include "evi_err.h"

/**
 * \cond INTERNAL_MACRO
 */
#define AXAL_LMM_DEFAULT_PRIO           (0)
#define AXAL_LMM_ALLOC_OVERHEAD         (sizeof(size_t))
/**
 * \endcond
 */

static struct axiom_al3sw_status {
    pthread_mutex_t mutex;
    evi_lmm_t almm;
} al3sw = { PTHREAD_MUTEX_INITIALIZER, };

static int
axiom_al3_sw_init(uintptr_t private_start, size_t private_size,
        uintptr_t shared_start, size_t shared_size)
{
    int ret;

    ret = evi_lmm_init(&al3sw.almm);
    if (ret != EVI_LMM_OK) {
        return AXAL_RET_ERROR;
    }

    if (private_size > 0) {
        ret = evi_lmm_add_reg(&al3sw.almm, (void *)private_start, private_size,
                EVI_PRIVATE_MEM, AXAL_LMM_DEFAULT_PRIO);
        if (ret != EVI_LMM_OK) {
            return AXAL_RET_ERROR;
        }
    }

    return AXAL_RET_OK;
}

static int
axiom_al3_sw_addshreg(uintptr_t block_addr, size_t block_size)
{
    int ret;

    pthread_mutex_lock(&al3sw.mutex);
    ret = evi_lmm_add_reg(&al3sw.almm, (void *)block_addr, block_size,
            EVI_SHARE_MEM, AXAL_LMM_DEFAULT_PRIO);
    pthread_mutex_unlock(&al3sw.mutex);

    return ret;
}

inline static void *
axiom_al3_sw_malloc(size_t sz, evi_region_flags_t flags)
{
    void *ptr;
    size_t nsize = sz + AXAL_LMM_ALLOC_OVERHEAD;

    pthread_mutex_lock(&al3sw.mutex);
    ptr = evi_lmm_alloc(&al3sw.almm, nsize, flags);

    if (ptr != NULL) {
	*((size_t *) ptr) = nsize;
	ptr = (void *)((uintptr_t)ptr + AXAL_LMM_ALLOC_OVERHEAD);
    }
    pthread_mutex_unlock(&al3sw.mutex);

    return ptr;
}

inline static void
axiom_al3_sw_free(void *ptr)
{
    void *p;

    if (ptr == NULL) {
	DPRINTF("Nothing to free: pointer is null!\n");
	return;
    }

    p = ((void *)ptr - AXAL_LMM_ALLOC_OVERHEAD);

    pthread_mutex_lock(&al3sw.mutex);
    if (*(size_t *)p > 0) {
	DPRINTF("Freeing %zu bytes\n", *(size_t *)p);
	evi_lmm_free(&al3sw.almm, p, *(size_t *)p);
    } else {
	EPRINTF("Invalid size: %p %zu\n", p, *(size_t *)p);
    }
    pthread_mutex_unlock(&al3sw.mutex);
}

static void *
axiom_al3_sw_prmalloc(size_t sz)
{
    return axiom_al3_sw_malloc(sz, EVI_PRIVATE_MEM);
}

static void *
axiom_al3_sw_shmalloc(size_t sz)
{
    return axiom_al3_sw_malloc(sz, EVI_SHARE_MEM);
}

static void
axiom_al3_sw_prfree(void *ptr)
{
    axiom_al3_sw_free(ptr);
}

static void
axiom_al3_sw_shfree(void *ptr)
{
    axiom_al3_sw_free(ptr);
}

static axiom_al3_info_t axiom_al3sw_info = {
    .init = axiom_al3_sw_init,
    .add_shregion = axiom_al3_sw_addshreg,
    .private_malloc = axiom_al3_sw_prmalloc,
    .shared_malloc = axiom_al3_sw_shmalloc,
    .private_free = axiom_al3_sw_prfree,
    .shared_free = axiom_al3_sw_shfree,
    .type = AXAL_SW,
    .overhead = AXAL_LMM_ALLOC_OVERHEAD,
};

static void axiom_al3sw_register(void) {
    axiom_al3_register(&axiom_al3sw_info);
}

module_init(axiom_al3sw_register);
