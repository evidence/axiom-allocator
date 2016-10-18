/*!
 * \file axiom_allocator_l2.h
 *
 * \version     v0.8
 * \date        2016-09-23
 *
 * This file contains the AXIOM allocator level 2 API
 */
#ifndef AXIOM_ALLOCATOR_L2_h
#define AXIOM_ALLOCATOR_L2_h

/*!
 * \brief Allocate unique Application ID through L1 allocator
 *
 * \param dev           AXIOM device to send RAW/LONG/RDMA messages
 * \param master_port   Port where you want receive the answer from L1 allocator
 * \param[out] app_id   Unique Application ID allocated
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_al2_alloc_appid(axiom_dev_t *dev, axiom_port_t master_port,
        axiom_app_id_t *app_id);

/*!
 * \brief Initialize the L2 allocator
 *
 * \param app_id        Application ID for this application
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_al2_init(axiom_app_id_t app_id);

/*!
 * \brief Release the the L2 and L1 allocator resources
 *
 * \param dev           AXIOM device to send RAW/LONG/RDMA messages
 */
void axiom_al2_release(axiom_dev_t *dev);

/*!
 * \brief Ask to L1 to allocate the shared and private regions. The info
 *        message (contained in the buffer) must specify the regions size.
 *
 * \param dev            AXIOM device to send RAW/LONG/RDMA messages
 * \param master_port    Port where you want receive the answer from L1 allocator
 * \param size           Size of buffer
 * \param[in|out] buffer Buffer that contains the allocator message from/to L3.
 *                       The input message must contain the appid, shared and
 *                       private size. The output message may contain the
 *                       error code.
 *
 * \return Return a value != 0 when the buffer contains the reply to L3.
 *         In case of error, the error code is contained in the buffer.
 */
int axiom_al2_alloc(axiom_dev_t *dev, axiom_port_t master_port, size_t size,
        void *buffer);

/*!
 * \brief
 * \param dev           AXIOM device to send RAW/LONG/RDMA messages
 * \param size          Size of buffer
 * \param buffer        Buffer that contains the allocator message from L1
 * \param[out] info     Buffer that contains the allocator message to reply to
 *                      L3 with the address and the size of the allocated
 *                      regions.
 *
 * \return Return a value != 0 when the buffer contains the reply to L3.
 *         In case of error, the error code is contained in the buffer.
 */
int axiom_al2_alloc_reply(axiom_dev_t *dev, size_t size, void *buffer,
        axiom_alloc_msg_t *info);

/*!
 * \brief Require the size and address of the private region.
 *
 * \param dev           AXIOM device to send RAW/LONG/RDMA messages
 * \param src_node      Node ID that requires a private region
 * \param size          Size of buffer
 * \param[out] buffer   Buffer that contains the allocator message from L3.
 *                      The output message will contain the size and the address
 *                      of the private region.
 *
 * \return Return a value != 0 when the buffer contains the reply to L3.
 *         In case of error, the error code is contained in the buffer.
 */
int axiom_al2_get_prblock(axiom_dev_t *dev, axiom_node_id_t src_node,
        size_t size, void *buffer);

/*!
 * \brief Require a new shared region.
 *
 * \param dev            AXIOM device to send RAW/LONG/RDMA messages
 * \param src_node       Node ID that requires a new shared blocks
 * \param size           Size of buffer
 * \param[in|out] buffer Buffer that contains the allocator message from/to L3.
 *                       The input message must contain the size of required
 *                       region. The output message will contain the size and
 *                       the address of the new shared blocks.
 *
 * \return Return a value != 0 when the buffer contains the reply to L3.
 *         In case of error, the error code is contained in the buffer.
 */
int axiom_al2_get_shblock(axiom_dev_t *dev, axiom_node_id_t src_node,
        size_t size, void *buffer);
#endif /* AXIOM_ALLOCATOR_L2_h */
