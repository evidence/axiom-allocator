/*!
 * \file axiom-allocator-test.c
 *
 * \version     v0.8
 * \date        2016-09-29
 *
 * This file contains the tests of axiom allocator
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "axiom_allocator.h"
#include "dprintf.h"

#define AXAL_TEST_PR_REGSIZE        (1 << 20)
#define AXAL_TEST_SH_REGSIZE        (1 << 22)
#define AXAL_TEST_PR_MALLOCSIZE     (1 << 10)
#define AXAL_TEST_SH_MALLOCSIZE     (1 << 12)
#define AXAL_TEST_TYPE          (AXAL_SW)

int verbose = 0;

static void
usage(void)
{
    printf("usage: axiom-allocator-test [arguments]\n");
    printf("Test the axiom-allocator API\n\n");
    printf("Arguments:\n");
    printf("-t, --type     sw|hw    type of allocator to use [default: SW]\n");
    printf("-v, --verbose           verbose\n");
    printf("-h, --help              print this help\n\n");
}


int
main(int argc, char **argv)
{
    axiom_altype_t axtype = AXAL_TEST_TYPE;
    size_t pr_regsize = AXAL_TEST_PR_REGSIZE;
    size_t sh_regsize = AXAL_TEST_SH_REGSIZE;
    size_t pr_mallocsize = AXAL_TEST_PR_MALLOCSIZE;
    size_t sh_mallocsize = AXAL_TEST_SH_MALLOCSIZE;
    void *pr_ptr, *sh_ptr;
    int long_index, opt, ret;

    static struct option long_options[] = {
            {"type", required_argument,  0, 't'},
            {"verbose", no_argument,     0, 'v'},
            {"help", no_argument,        0, 'h'},
            {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv,"t:vh",
                         long_options, &long_index)) != -1) {
        char *type_string = NULL;

        switch (opt) {
            case 't':
                if ((sscanf(optarg, "%ms", &type_string) != 1) ||
                        type_string == NULL) {
                    EPRINTF("wrong message type");
                    usage();
                    exit(-1);
                }

                if (strncmp(type_string, "sw", 2) == 0) {
                    axtype = AXAL_SW;
                } else if (strncmp(type_string, "hw", 2) == 0) {
                    axtype = AXAL_HW;
                } else {
                    EPRINTF("wrong message type");
                    free(type_string);
                    usage();
                    exit(-1);
                }

                free(type_string);
                break;

            case 'v':
                verbose = 1;
                break;

            case 'h':
            default:
                usage();
                exit(-1);
        }
    }

    ret = axiom_allocator_init(pr_regsize, sh_regsize, axtype);
    if (ret) {
        EPRINTF("axiom_allocator_init error %d", ret);
    }
    IPRINTF(verbose, "axiom-allocator initialized");

    pr_ptr = axiom_private_malloc(pr_mallocsize);
    if (pr_ptr == NULL) {
        EPRINTF("axiom_private_malloc error %p", pr_ptr);
    }
    IPRINTF(verbose, "private memory allocated - addr: %p size: %zu",
            pr_ptr, pr_mallocsize);

    sh_ptr = axiom_shared_malloc(sh_mallocsize);
    if (sh_ptr == NULL) {
        EPRINTF("axiom_shared_malloc error %p", sh_ptr);
    }
    IPRINTF(verbose, "shared memory allocated - addr: %p size: %zu",
            sh_ptr, sh_mallocsize);

    axiom_private_free(pr_ptr);
    IPRINTF(verbose, "private memory freed - addr: %p size: %zu",
            pr_ptr, pr_mallocsize);
    axiom_shared_free(sh_ptr);
    IPRINTF(verbose, "shared memory freed - addr: %p size: %zu",
            sh_ptr, sh_mallocsize);

    return 0;
}
