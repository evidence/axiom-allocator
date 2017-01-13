/*!
 * \file axiom_allocator_l1.h
 *
 * \version     v0.10
 * \date        2016-09-23
 *
 * This file contains the AXIOM allocator level 1 API
 */
#ifndef AXIOM_ALLOCATOR_L1_h
#define AXIOM_ALLOCATOR_L1_h

#include "axiom_allocator_protocol.h"

/*!
 * \defgroup AXIOM_ALLOCATOR
 *
 * \{
 */

/*!
 * \brief Initialize the L1 allocator
 */
void axiom_al1_init(void);

/*!
 * \brief Allocate a new unique Application ID in the system.
 *
 * \param[out] info     The output message will contain the application ID.
 *
 * \return Return a value != 0 when the info message contains the reply to L2.
 *         In case of error, the error code is contained in the info message.
 */
int axiom_al1_alloc_appid(axiom_alloc_msg_t *info);

/*!
 * \brief Allocate the shared and private regions in L1 and set the address and
 *        size aligned of the regions in the info message to reply to L2
 *
 * \param[in,out] info  The input message must contain the appid, shared and
 *                      private size. The output message will contain the
 *                      address and the size of the allocated regions.
 *
 * \return Return a value != 0 when the info message contains the reply to L2.
 *         In case of error, the error code is contained in the info message.
 */
int axiom_al1_alloc(axiom_alloc_msg_t *info);

/*!
 * \brief Release the Application ID and the shared and private regions
 *        previously allocated.
 *
 * \param[in,out] info The input message must contain the appid.
 *
 * \return Return a value != 0 when the info message contains the reply to L2.
 *         In case of error, the error code is contained in the info message.
 */
int axiom_al1_release(axiom_alloc_msg_t *info);

/** \} */

#endif /* AXIOM_ALLOCATOR_L1_h */
