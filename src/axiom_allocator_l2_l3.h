/*!
 * \file axiom_allocator_l2_l3.h
 *
 * \version     v0.14
 * \date        2016-09-29
 *
 * This file contains the AXIOM allocator interface between L2 and L3
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#ifndef AXIOM_ALLOCATOR_L2_L3_h
#define AXIOM_ALLOCATOR_L2_L3_h

/**
 * \defgroup AXIOM_ALLOCATOR
 *
 * \{
 */

/*! \brief Axiom allocator: L2 block size       1 MB */
#define AXIOM_ALLOCATOR_L2_BSIZE                (1 << 20)

/*!
 * \brief Initialize the L2 allocator.
 *
 * \param private_size	maximum private memory size needed by the application
 * \param shared_size   maximum shared memory size needed by the application
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_al23_init(size_t private_size, size_t shared_size);

/*!
 * \brief Get the private and shared regions.
 *        If it is called multiple times, it returns the same regions.
 *
 * \param start         start address of region
 * \param size          size of region
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_al23_get_regions(uintptr_t *private_start, size_t *privare_size,
        uintptr_t *shared_start, size_t *shared_size);

/*!
 * \brief Alloc a new shared block.
 *
 * \param start         start address of block
 * \param[inout] size   size of block
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_al23_alloc_shblock(uintptr_t *start, size_t *size);


/*!
 * \brief Get the application ID.
 *
 * \return Return the application ID or error code of operation.
 *         (>=0 if everything is OK)
 */
int axiom_al23_get_appid(void);

/** \} */

#endif /* !AXIOM_L2_ALLOCATOR_h */
