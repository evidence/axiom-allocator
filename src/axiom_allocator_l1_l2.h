/*!
 * \file axiom_allocator_l1_l2.h
 *
 * \version     v0.10
 * \date        2016-09-23
 *
 * This file contains the AXIOM allocator interface between level 1 and level 2
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#ifndef AXIOM_ALLOCATOR_L1_L2_h
#define AXIOM_ALLOCATOR_L1_L2_h

/**
 * \defgroup AXIOM_ALLOCATOR
 *
 * \{
 */

#include "axiom_allocator_protocol.h"

/*! \brief Axiom allocator: RDMA memory size    1 GB */
#define AXIOM_ALLOCATOR_MEM_SIZE                (1 << 30)
/*! \brief Axiom allocator: RDMA memory start */
#define AXIOM_ALLOCATOR_MEM_START               0
/*! \brief Axiom allocator: L1 block size       16 MB */
#define AXIOM_ALLOCATOR_L1_BSIZE                (1 << 24)
/*! \brief Axiom allocator: number of app id available */
#define AXIOM_ALLOCATOR_NUM_APP_ID              255

/*
 * \brief Request an unique application ID to the AXIOM_MASTER_NODE_INIT
 *
 * \param dev           Axiom device private data pointer
 * \param reply_port    Port where you want to receive the reply
 *
 * \return Returns AXIOM_RET_OK on success, an error otherwise.
 */
axiom_err_t
axiom_al12_get_appid(axiom_dev_t *dev, axiom_port_t reply_port);

/*
 * \brief Waiting for a reply from AXIOM_MASTER_NODE_INIT with an
 *        unique application ID
 *
 * \param dev           Axiom device private data pointer
 * \param[out] appid    Application ID received for the application
 *
 * \return Returns AXIOM_RET_OK on success, an error otherwise.
 */
axiom_err_t
axiom_al12_get_appid_reply(axiom_dev_t *dev, axiom_app_id_t *appid);


/*
 * \brief Request to the AXIOM_MASTER_NODE_INIT to allocate private and
 *        shared regions for the specified application (app_id)
 *
 * \param dev               Axiom device private data pointer
 * \param reply_port        Port where you want to receive the reply
 * \param appid             Application ID of the application
 * \param private_size      Size of private region (return size block
 *                          aligned)
 * \param shared_size       Size of private region (return size block
 *                          aligned)
 *
 * \return Returns AXIOM_RET_OK on success, an error otherwise.
 */
axiom_err_t
axiom_al12_alloc(axiom_dev_t *dev, axiom_port_t reply_port, axiom_app_id_t appid,
        size_t private_size, size_t shared_size);


axiom_err_t
axiom_al12_alloc_parsereply(void *payload, size_t payload_size,
        uintptr_t *private_start, size_t *private_size, uintptr_t *shared_start,
        size_t *shared_size);

/*
 * \brief Request to the AXIOM_MASTER_NODE_INIT to release the application
 *        ID and the private and shared regions.
 *
 * \param dev               Axiom device private data pointer
 * \param appid             Application ID of the application
 *
 * \return Returns AXIOM_RET_OK on success, an error otherwise.
 */
axiom_err_t
axiom_al12_release(axiom_dev_t *dev, axiom_app_id_t appid);

/** \} */

#endif /* AXIOM_ALLOCATOR_L1_L2_h */
