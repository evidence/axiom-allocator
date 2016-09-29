/*!
 * \file axiom_l2_allocator.h
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the AXIOM L2 allocator API
 *
 */
#ifndef AXIOM_L2_ALLOCATOR_h
#define AXIOM_L2_ALLOCATOR_h

/*!
 * \brief Initialize the L2 allocator.
 *
 * \param master_nodeid Node id of master node
 * \param local_nodeid  Node id of local node
 * \param private_size	maximum private memory size needed by the application
 * \param shared_size   maximum shared memory size needed by the application
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_l2_init(uint8_t master_nodeid, uint8_t local_nodeid,
        uint64_t private_size, uint64_t shared_size);

/*!
 * \brief Get the private region. If it is called multiple times, it returns
 *        the same region.
 *
 * \param start         start address of region
 * \param size          size of region
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_l2_get_prregion(uint64_t *start, uint64_t *size);

/*!
 * \brief Require a new shared region.
 *
 * \param start         start address of region
 * \param[inout] size   size of region
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_l2_req_shregion(uint64_t *start, uint64_t *size);

#endif /* !AXIOM_L2_ALLOCATOR_h */
