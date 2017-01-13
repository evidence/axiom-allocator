/*!
 * \file axiom_allocator_protocol.h
 *
 * \version     v0.10
 * \date        2016-09-26
 *
 * This file contains the AXIOM message exchanged between levels to implement
 * the allocator API
 *
 */
#ifndef AXIOM_ALLOCATOR_PROTOCOL_h
#define AXIOM_ALLOCATOR_PROTOCOL_h

/* environment variables exported by axiom-run to the applications */
/*! \brief node ID where runs the master application */
#define AXIOM_ENV_ALLOC_APPMASTER       "AXIOM_ALLOC_APPMASTER"

typedef struct axiom_alloc_msg {
    uint64_t shared_start;      /*!< \brief Start address of shared region */
    uint64_t shared_size;       /*!< \brief Size of shared region */
    uint64_t private_start;     /*!< \brief Start address of private region */
    uint64_t private_size;      /*!< \brief Size of private region */
    uint8_t  app_id;            /*!< \brief Application ID */
    int8_t   error;             /*!< \brief axiom_err_t error */
    uint8_t  spare[7];
} axiom_alloc_msg_t;

/*! \brief Message payload for the axiom allocator */
typedef struct axiom_allocator_payload {
    uint8_t  command;           /*!< \brief Command of allocator messages */
    uint8_t  reply_port;        /*!< \brief Port where to reply */
    uint8_t  padding[5];
    axiom_alloc_msg_t info;
} axiom_allocator_payload_t;

#endif /* !AXIOM_ALLOCATOR_PROTOCOL_h */
