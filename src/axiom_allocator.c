/*!
 * \file axiom_allocator.c
 *
 * \version     v0.11
 * \date        2016-09-29
 *
 * This file contains the implementation of axiom-allocator API.
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stropts.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <axiom_mem_dev_user.h>

#include "axiom_allocator.h"
#include "axiom_allocator_l3/axiom_allocator_l3.h"
#include "axiom_allocator_l2_l3.h"
#include "dprintf.h"

static axiom_al3_info_t al3_info;

#define MAX_INFO_TABLE  2
static axiom_al3_info_t info_table[MAX_INFO_TABLE];
static int info_table_used = 0;

static struct axiom_allocator_s {
    int mem_dev_fd;
    uintptr_t vaddr_start;
    uintptr_t vaddr_end;
} al3_status;

#define USE_EXT_LDSCRIPT
#ifdef USE_EXT_LDSCRIPT
extern unsigned long __ld_shm_info_start_addr;
extern unsigned long __ld_shm_info_end_addr;
#else
unsigned long __ld_shm_info_start_addr = 0x4000000000;
unsigned long __ld_shm_info_end_addr = 0x4040000000;
#endif

void
axiom_al3_register(axiom_al3_info_t *info)
{
    DPRINTF("registered new impl - type: %d", info->type);

    if (info_table_used < MAX_INFO_TABLE) {
        info_table[info_table_used++] = *info;
    }
}

static axiom_al3_info_t *
axiom_al3_find_type(axiom_altype_t type)
{
    int i;

    for (i = 0; i < info_table_used; i++) {
        if (info_table[i].type == type) {
            return &info_table[i];
        }
    }

    return NULL;
}

typedef void (*call_t)(void);

extern call_t axiom_allocator_start[];
extern call_t axiom_allocator_end[];

int
axiom_allocator_init(size_t *private_size, size_t *shared_size,
        axiom_altype_t type)
{
    uintptr_t private_start, shared_start;
    struct axiom_mem_dev_info memreq;
    struct axiom_mem_dev_app memapp;
    axiom_al3_info_t *info;
    int ret, appid;

    /* call L3 registration functions */
    /* rationale: resolve C++ initialization FIASCO */
    call_t *f;
    call_t fn;
    for (f = axiom_allocator_start; f < axiom_allocator_end; f++) {
        fn = *f;
        fn();
    }

    /* find L3 type */
    info = axiom_al3_find_type(type);
    if (info == NULL) {
        EPRINTF("type %d not found!", type);
        return AXAL_RET_ERROR;
    }
    al3_info = *info;

    /* init L2 allocator */
    ret = axiom_al23_init(*private_size, *shared_size);
    if (ret) {
        EPRINTF("axiom_al23_init - ret: %d", ret);
        return AXAL_RET_ERROR;
    }

    /* take private and shared region from L2 allocator */
    ret = axiom_al23_get_regions(&private_start, private_size, &shared_start,
            shared_size);
    if (ret) {
        EPRINTF("axiom_al23_get_regions - ret: %d", ret);
        return AXAL_RET_ERROR;
    }

    /* take the application ID from L2 allocator */
    appid = axiom_al23_get_appid();
    if (appid < 0) {
        EPRINTF("axiom_al23_get_appid - ret: %d", appid);
        return AXAL_RET_ERROR;
    }

    /* open axiom memory device to configure the mapping */
    ret = open("/dev/axiom_dev_mem0", O_RDWR);
    if (ret < 0) {
        perror("open");
        return AXAL_RET_ERROR;
    }
    al3_status.mem_dev_fd = ret;

    al3_status.vaddr_start = (uintptr_t)__ld_shm_info_start_addr;
    al3_status.vaddr_end = (uintptr_t)__ld_shm_info_end_addr;

    /* add the vaddr offset to the addresses */
    private_start += al3_status.vaddr_start;
    shared_start += al3_status.vaddr_start;

#ifdef USE_EXT_LDSCRIPT
    ret = mprotect((void *)(al3_status.vaddr_start), al3_status.vaddr_end -
            al3_status.vaddr_start, PROT_NONE);
    if (ret) {
	perror("mprotect");
	return ret;
    }
#endif

    /* set up the memory mapping */
    memreq.base = al3_status.vaddr_start;
    memreq.size = al3_status.vaddr_end - al3_status.vaddr_start;
    ret = ioctl(al3_status.mem_dev_fd, AXIOM_MEM_DEV_CONFIG_VMEM, &memreq);
    if (ret) {
	perror("ioctl");
	return ret;
    }

    memapp.app_id = appid;
    memapp.used_regions = 0;

    /* map the private region in the virtual address */
    if (*private_size > 0) {
        memapp.info[memapp.used_regions].base = private_start;
        memapp.info[memapp.used_regions].size = *private_size;
        memapp.used_regions++;
    }

    /* map the shared region in the virtual address */
    if (*shared_size > 0) {
        memapp.info[memapp.used_regions].base = shared_start;
        memapp.info[memapp.used_regions].size = *shared_size;
        memapp.used_regions++;
    }

    /* set the application ID and regions */
    ret = ioctl(al3_status.mem_dev_fd, AXIOM_MEM_DEV_SET_APP, &memapp);
    if (ret) {
	perror("ioctl");
	return ret;
    }

    return al3_info.init(private_start, *private_size, shared_start,
            *shared_size);
}

void *
axiom_private_malloc(size_t sz)
{
    return al3_info.private_malloc(sz);
}

void *
axiom_shared_malloc(size_t sz)
{
    uintptr_t block_addr;
    size_t block_size;
    void *addr;
    int ret;

    /* TODO: Do we need to add a mutex to protect this function? */
    addr = al3_info.shared_malloc(sz);
    if (addr) {
        return addr;
    }

    /*
     * if L3 malloc fails, request a new shared block from L2 allocator
     *
     * NOTE: L3 need enough space to store the size of allocated memory, so
     * sizeof(sz) is ok.
     */
    block_size = sz + sizeof(sz);
    ret = axiom_al23_alloc_shblock(&block_addr, &block_size);
    if (ret) {
        return NULL;
    }

    /* add the new block to L3 allocator */
    ret = al3_info.add_shregion(block_addr + al3_status.vaddr_start,
            block_size);
    if (ret) {
        return NULL;
    }

    /* retry L3 allocation */
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
