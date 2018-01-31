/*!
 * \file axiom-allocator-test.c
 *
 * \version     v1.0
 * \date        2016-09-29
 *
 * This file contains the tests of axiom allocator
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <getopt.h>

#include "axiom_allocator.h"
#include "dprintf.h"

#define AXAL_TEST_PR_REGSIZE            (1 << 20)
#define AXAL_TEST_SH_REGSIZE            (1 << 22)
#define AXAL_TEST_PR_MALLOCSIZE         (1 << 10)
#define AXAL_TEST_SH_MALLOCSIZE         (1 << 12)
#define AXAL_TEST_TYPE                  AXAL_SW
#define AXAL_TEST_MAGIC                 0x33
#define AXAL_TEST_NMALLOC               3
#define AXAL_TEST_NMALLOC_MAX           64

int verbose = 0;

static void
usage(void)
{
    printf("usage: axiom-allocator-test [arguments]\n");
    printf("Test the axiom-allocator API\n\n");
    printf("Arguments:\n");
    printf("-t, --type      sw|hw       type of allocator to use [default: SW]\n");
    printf("-n, --nmalloc   num         number of malloc to do [default: %d]\n",
            AXAL_TEST_NMALLOC);
    printf("-b, --magicbyte maigc       magic byte written after the malloc [default: 0x%x]\n",
            AXAL_TEST_MAGIC);
    printf("-R, --sregsize  x[B|K|M|G]  size of shared region [default: %d B]\n",
            AXAL_TEST_SH_REGSIZE);
    printf("-r, --pregsize  x[B|K|M|G]  size of private region [default: %d B]\n",
            AXAL_TEST_PR_REGSIZE);
    printf("-M, --smalsize  x[B|K|M|G]  size of shared malloc [default: %d B]\n",
            AXAL_TEST_SH_MALLOCSIZE);
    printf("-m, --pmalsize  x[B|K|M|G]  size of private malloc [default: %d B]\n",
            AXAL_TEST_PR_MALLOCSIZE);
    printf("-v, --verbose               verbose\n");
    printf("-h, --help                  print this help\n\n");
}

static int
get_scale(char char_scale) {
    int scale;

    switch (char_scale) {
        case 'b': case 'B':
            scale = 0;
            break;

        case 'k': case 'K':
            scale = 10;
            break;

        case 'm': case 'M':
            scale = 20;
            break;

        case 'G': case 'g':
            scale = 30;
            break;

        default:
            scale = 0;
    }

    return scale;
}


static int
axal_test_checkmem(uint8_t *addr, size_t len, uint8_t magic)
{
    int i;

    for (i = 0; i < len; i++) {
        if(*addr != magic) {
            EPRINTF("check failed at 0x%p, expected 0x%x instead of 0x%x",
                    addr, magic, *addr);
            return -4;
        }
        addr++;
    }

    return 0;
}

int
main(int argc, char **argv)
{
    axiom_altype_t altype = AXAL_TEST_TYPE;
    size_t pr_regsize = AXAL_TEST_PR_REGSIZE;
    size_t sh_regsize = AXAL_TEST_SH_REGSIZE;
    size_t pr_mallocsize = AXAL_TEST_PR_MALLOCSIZE;
    size_t sh_mallocsize = AXAL_TEST_SH_MALLOCSIZE;
    int nmalloc = AXAL_TEST_NMALLOC, i;
    uint8_t magic = AXAL_TEST_MAGIC;
    void *pr_ptr[AXAL_TEST_NMALLOC_MAX], *sh_ptr[AXAL_TEST_NMALLOC_MAX];
    int long_index, opt, ret;

    static struct option long_options[] = {
            {"type", required_argument,  0, 't'},
            {"nmalloc", required_argument,  0, 'n'},
            {"magicbyte", required_argument,  0, 'b'},
            {"sregsize", required_argument,  0, 'R'},
            {"pregsize", required_argument,  0, 'r'},
            {"smalsize", required_argument,  0, 'M'},
            {"pmalsize", required_argument,  0, 'm'},
            {"verbose", no_argument,     0, 'v'},
            {"help", no_argument,        0, 'h'},
            {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv,"t:n:R:r:M:m:vh",
                         long_options, &long_index)) != -1) {
        char *type_string = NULL, cscale = 'B';
        uint64_t buf64;

        switch (opt) {
            case 't':
                if ((sscanf(optarg, "%ms", &type_string) != 1) ||
                        type_string == NULL) {
                    EPRINTF("wrong message type");
                    usage();
                    exit(-1);
                }

                if (strncmp(type_string, "sw", 2) == 0) {
                    altype = AXAL_SW;
                } else if (strncmp(type_string, "hw", 2) == 0) {
                    altype = AXAL_HW;
                } else {
                    EPRINTF("wrong message type");
                    free(type_string);
                    usage();
                    exit(-1);
                }

                free(type_string);
                break;

            case 'n':
                if (sscanf(optarg, "%d", &nmalloc) == 0) {
                    EPRINTF("wrong input");
                    usage();
                    exit(-1);
                }
                break;

            case 'b':
                if (sscanf(optarg, "%" SCNu8, &magic) == 0) {
                    EPRINTF("wrong input");
                    usage();
                    exit(-1);
                }
                break;

            case 'R':
                if (sscanf(optarg, "%" SCNu64 "%c", &buf64, &cscale) == 0) {
                    EPRINTF("wrong input");
                    usage();
                    exit(-1);
                }
                sh_regsize = buf64 << get_scale(cscale);
                break;

            case 'r':
                if (sscanf(optarg, "%" SCNu64 "%c", &buf64, &cscale) == 0) {
                    EPRINTF("wrong input");
                    usage();
                    exit(-1);
                }
                pr_regsize = buf64 << get_scale(cscale);
                break;

            case 'M':
                if (sscanf(optarg, "%" SCNu64 "%c", &buf64, &cscale) == 0) {
                    EPRINTF("wrong input");
                    usage();
                    exit(-1);
                }
                sh_mallocsize = buf64 << get_scale(cscale);
                break;

            case 'm':
                if (sscanf(optarg, "%" SCNu64 "%c", &buf64, &cscale) == 0) {
                    EPRINTF("wrong input");
                    usage();
                    exit(-1);
                }
                pr_mallocsize = buf64 << get_scale(cscale);
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

    if (nmalloc > AXAL_TEST_NMALLOC_MAX) {
        EPRINTF("nmalloc[%d] must be <= %d", nmalloc, AXAL_TEST_NMALLOC);
        usage();
        exit(-1);
    }
    memset(&pr_ptr[0], 0, sizeof(pr_ptr[0]) * AXAL_TEST_NMALLOC_MAX);
    memset(&sh_ptr[0], 0, sizeof(sh_ptr[0]) * AXAL_TEST_NMALLOC_MAX);

    printf("-- start axiom allocator test --\n");
    printf("   allocator type (SW=1, HW=2) = %d\n", altype);
    printf("   num_malloc = %d magic = 0x%x\n", nmalloc, magic);
    printf("   private - request region_size = %zu malloc_size = %zu\n",
            pr_regsize, pr_mallocsize);
    printf("   shared  - request region_size = %zu malloc_size = %zu\n",
            sh_regsize, sh_mallocsize);

    ret = axiom_allocator_init(&pr_regsize, &sh_regsize, altype);
    if (ret) {
        EPRINTF("axiom_allocator_init error %d", ret);
        ret = -1;
        goto err;
    }
    printf("-- axiom allocator initialized --\n");
    printf("   private - allocated region_size = %zu\n", pr_regsize);
    printf("   shared  - allocated region_size = %zu\n", sh_regsize);

    for (i = 0; i < nmalloc; i++) {
        pr_ptr[i] = axiom_private_malloc(pr_mallocsize);
        if (pr_ptr[i] == NULL) {
            EPRINTF("axiom_private_malloc error %p", pr_ptr[i]);
            ret = -2;
            goto err;
        }
        IPRINTF(verbose, "private memory allocated - addr: %p size: %zu",
                pr_ptr[i], pr_mallocsize);

        memset(pr_ptr[i], magic, pr_mallocsize);
        IPRINTF(verbose, "private memory set to 0x%x", magic);

        sh_ptr[i] = axiom_shared_malloc(sh_mallocsize);
        if (sh_ptr[i] == NULL) {
            EPRINTF("axiom_shared_malloc error %p", sh_ptr[i]);
            ret = -3;
            goto err;
        }
        IPRINTF(verbose, "shared memory allocated - addr: %p size: %zu",
                sh_ptr[i], sh_mallocsize);

        memset(sh_ptr[i], magic, sh_mallocsize);
        IPRINTF(verbose, "shared memory set to 0x%x", magic);
    }

    for (i = 0; i < nmalloc; i++) {
        uint8_t *addr;
        int b;
        if (pr_ptr[i]) {
            ret += axal_test_checkmem(pr_ptr[i], pr_mallocsize, magic);
            IPRINTF(verbose, "private memory checked - ret %d", ret);

            axiom_private_free(pr_ptr[i]);
            IPRINTF(verbose, "private memory freed - addr: %p size: %zu",
                    pr_ptr[i], pr_mallocsize);
        }
        if (sh_ptr[i]) {
            ret += axal_test_checkmem(sh_ptr[i], sh_mallocsize, magic);
            IPRINTF(verbose, "shared memory checked - ret %d", ret);

            axiom_shared_free(sh_ptr[i]);
            IPRINTF(verbose, "shared memory freed - addr: %p size: %zu",
                    sh_ptr[i], sh_mallocsize);
        }
    }

err:
    printf("-- ended with error code %d --\n", ret);
    return ret;
}
