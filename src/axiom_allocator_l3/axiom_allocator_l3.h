/*!
 * \file axiom_allocator_l3.h
 *
 * \version     v0.15
 * \date        2016-09-29
 *
 * This file contains the AXIOM L3 allocator skeleton API
 *
 * Copyright (C) 2016, Evidence Srl.
 * Terms of use are as specified in COPYING
 */
#ifndef AXIOM_L3_ALLOCATOR_h
#define AXIOM_L3_ALLOCATOR_h

/**
 * \defgroup AXIOM_ALLOCATOR
 *
 * \{
 */

typedef int (*axiom_al3_init_t)(uintptr_t private_start, size_t private_size,
        uintptr_t shared_start, size_t shared_size);
typedef int (*axiom_al3_addshreg_t)(uintptr_t block_addr, size_t block_size);
typedef void * (*axiom_al3_prmalloc_t)(size_t sz);
typedef void * (*axiom_al3_shmalloc_t)(size_t sz);
typedef void (*axiom_al3_shfree_t)(void *ptr);
typedef void (*axiom_al3_prfree_t)(void *ptr);

/*!
 * \brief Structure with function pointers of L3 allocator implementation
 */
typedef struct {
    axiom_altype_t type;                /*!< \brief type of allocator */
    axiom_al3_init_t init;              /*!< \brief init function of allocator */
    axiom_al3_addshreg_t add_shregion;  /*!< \brief add new shared region to the allocator */
    axiom_al3_prmalloc_t private_malloc;/*!< \brief malloc for the private reg */
    axiom_al3_shmalloc_t shared_malloc; /*!< \brief malloc for the shared reg */
    axiom_al3_prfree_t private_free;    /*!< \brief free for the private reg */
    axiom_al3_shfree_t shared_free;     /*!< \brief free for the shared reg */
    size_t overhead;                    /*!< \brief overhead used in each region */
} axiom_al3_info_t;

/*!
 * \brief Function used to register a new implementation of L3 API
 *
 * \param info  function pointers of L3 allocator implementation
 */
void axiom_al3_register(axiom_al3_info_t *info);



#define module_init(function) \
 __attribute__ ((section (".init.axiom_allocator"))) void *axiom_init_func_ ## function = function;

/** \} */

#endif /* !AXIOM_L3_ALLOCATOR_h */
