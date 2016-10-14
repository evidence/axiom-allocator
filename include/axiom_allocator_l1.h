/*!
 * \file axiom_allocator_l1.h
 *
 * \version     v0.8
 * \date        2016-09-23
 *
 * This file contains the AXIOM allocator level 1
 */
#ifndef AXIOM_ALLOCATOR_L1_h
#define AXIOM_ALLOCATOR_L1_h

void axiom_al1_init(void);
int axiom_al1_alloc(axiom_galloc_info_t *info);
int axiom_al1_alloc_appid(axiom_galloc_info_t *info);
int axiom_al1_release(axiom_galloc_info_t *info);

#endif /* AXIOM_ALLOCATOR_L1_h */
