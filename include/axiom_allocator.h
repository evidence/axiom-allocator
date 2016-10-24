#ifndef AXIOM_ALLOCATOR_H
#define AXIOM_ALLOCATOR_H
/*!
 * \file axiom_allocator.h
 *
 * \version     v0.8
 * \date        2016-09-21
 *
 * This file contains the AXIOM Allocator API
 *
 */

/**
 * \defgroup AXIOM_ALLOCATOR
 *
 * \{
 */

/*! \brief Return value OK */
#define AXAL_RET_OK             0
/*! \brief Return value ERROR: generic error */
#define AXAL_RET_ERROR          -1
/*! \brief Return value NOMEM: memory not available */
#define AXAL_RET_NOMEM          -2

/*! \brief Type of L3 allocator to use
 *
 *  AXAL_SW = software implementation based on LMM
 *  AXAL_HW = hardware implementation on the FPGA
 */
typedef enum {AXAL_NONE = 0, AXAL_SW, AXAL_HW} axiom_altype_t;

/**
 * \brief Initialize the allocator.
 *
 * \param private_size	maximum private memory size needed by the application
 * \param shared_size   maximum shared memory size needed by the application
 * \param type          type of allocator to use
 *
 * \return Return the error code of operation. (0 if everything is OK)
 */
int axiom_allocator_init(size_t private_size, size_t shared_size,
        axiom_altype_t type);

/**
 * \brief Allocate memory of sz bytes in the private region
 *
 * \param sz  Size of the required memory
 *
 * \return Return the pointer of the allocated memory. NULL in case of error.
 */
void *axiom_private_malloc(size_t sz);

/**
 * \brief Allocate memory of sz bytes in the shared region
 *
 * \param sz  Size of the required memory
 *
 * \return Return the pointer of the allocated memory. NULL in case of error.
 */
void *axiom_shared_malloc(size_t sz);

/**
 * \brief Free a previous allocated memory in the private region
 *
 * \param ptr  Pointer to the memory to free
 */
void axiom_private_free(void *ptr);

/**
 * \brief Free a previous allocated memory in the shared region
 *
 * \param ptr  Pointer to the memory to free
 */
void axiom_shared_free(void *ptr);

/** \} */

#endif /* AXIOM_ALLOCATOR_H */
