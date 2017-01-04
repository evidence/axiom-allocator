/*!
 * \file axiom_allocator_l2_l3.c
 *
 * \version     v0.9
 * \date        2016-09-29
 *
 * This file contains the implementation of AXIOM L2 L3 allocator interface.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../axiom_allocator_l2_l3.h"
#include "dprintf.h"
#include "axiom_nic_types.h"
#include "axiom_nic_limits.h"
#include "axiom_nic_api_user.h"
#include "axiom_run_api.h"
#include "axiom_allocator_protocol.h"

#define AXIOM_ALLOCATOR_BARRIER_ID      3

static int
axiom_al23_get_nodeids(axiom_node_id_t *master_nodeid,
        axiom_node_id_t *local_nodeid)
{
    axiom_dev_t *dev;
    char *nodeid_s;
    unsigned long nodeid = 0;

    nodeid_s = getenv(AXIOM_ENV_ALLOC_APPMASTER);
    if (nodeid_s == NULL) {
        EPRINTF("env %s not defined", AXIOM_ENV_ALLOC_APPMASTER);
        return -1;
    }

    nodeid = strtoul(nodeid_s, NULL, 10);
    *master_nodeid = (axiom_node_id_t)(nodeid);

    dev = axiom_open(NULL);
    if (dev == NULL) {
        return -1;
    }

    *local_nodeid = axiom_get_node_id(dev);

    axiom_close(dev);

    return 0;
}

int
axiom_al23_init(size_t private_size, size_t shared_size)
{
    axiom_err_t ret = AXIOM_RET_OK, err;
    axiom_node_id_t master_nodeid, local_nodeid;
    int verbose = 0;

    err = axiom_al23_get_nodeids(&master_nodeid, &local_nodeid);
    if (err < 0) {
        EPRINTF("axiom_al23_get_nodeids error %d", err);
        return AXIOM_RET_ERROR;
    }

    if (local_nodeid == master_nodeid) {
        axiom_alloc_msg_t info;
        size_t info_size = sizeof(info);

        info.private_size = private_size;
        info.shared_size = shared_size;

        /* alloc private and shared regions */
        err = axrun_rpc(AXRUN_RPC_ALLOC, info_size, &info, &info_size, &info,
                verbose);
        if (err < 0 || !AXIOM_RET_IS_OK(info.error)) {
            EPRINTF("axrun_rpc ret %d info.error %d", err, info.error);
            ret = AXIOM_RET_NOMEM;
        }

    }

    /* barrier for all nodes */
    err = axrun_sync(AXIOM_ALLOCATOR_BARRIER_ID, verbose);
    if (err < 0) {
        EPRINTF("axrun_sync error %d", err);
        ret = AXIOM_RET_ERROR;
    }

    return ret;
}

int
axiom_al23_get_regions(uintptr_t *private_start, size_t *private_size,
        uintptr_t *shared_start, size_t *shared_size)
{
    axiom_alloc_msg_t info;
    size_t info_size = sizeof(info);
    axiom_err_t ret;
    int verbose = 0;

    ret = axrun_rpc(AXRUN_RPC_GET_REGIONS, info_size, &info, &info_size, &info,
            verbose);
    if (ret < 0 || !AXIOM_RET_IS_OK(info.error)) {
        EPRINTF("axrun_rpc ret %d info.error %d", ret, info.error);
        return AXIOM_RET_NOMEM;
    }

    //fprintf(stderr,"HERE 0x%lx %d %d\n",info.private_start,sizeof(uint64_t),sizeof(uintptr_t));
    
    *private_start = info.private_start;
    *private_size = info.private_size;
    *shared_start = info.shared_start;
    *shared_size = info.shared_size;

    return AXIOM_RET_OK;
}

int
axiom_al23_alloc_shblock(uintptr_t *start, size_t *size)
{
    axiom_alloc_msg_t info;
    size_t info_size = sizeof(info);
    axiom_err_t ret;
    int verbose = 0;

    info.shared_size = *size;

    ret = axrun_rpc(AXRUN_RPC_ALLOC_SHBLOCK, info_size, &info, &info_size, &info,
            verbose);
    if (ret < 0 || !AXIOM_RET_IS_OK(info.error)) {
        EPRINTF("axrun_rpc ret %d info.error %d", ret, info.error);
        return AXIOM_RET_NOMEM;
    }

    *start = info.shared_start;
    *size = info.shared_size;

    return AXIOM_RET_OK;
}

int
axiom_al23_get_appid(void)
{
    char *appid_s;
    unsigned long appid = 0;

    appid_s = getenv(AXIOM_ENV_ALLOC_APPID);
    if (appid_s == NULL) {
        EPRINTF("env %s not defined", AXIOM_ENV_ALLOC_APPID);
        return AXIOM_RET_ERROR;
    }

    appid = strtoul(appid_s, NULL, 10);

    return (axiom_app_id_t)(appid);
}
