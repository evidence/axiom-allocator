#AM_CFLAGS = -Wall -fPIC -I$(top_srcdir)/include -I@AXIOM_EVI_NIC_INCLUDE@ -I@AXIOM_EVI_APPS_INCLUDE@ -I@EVI_ALLOCATOR_INCLUDE@ -I@AXIOM_MEM_DEV_INC@
AM_CFLAGS = -Wall -fPIC -I$(top_srcdir)/include @axiom_libraries_CFLAGS@ @evidence_lmm_CFLAGS@ -I@PKG_CONFIG_SYSROOT_DIR@/usr/include
#-I@EVI_ALLOCATOR_INCLUDE@

if DEBUG
AM_CFLAGS += -DPDEBUG
endif

#AM_LDFLAGS = -Wl,--verbose -Wl,-T./allocator.lds
AM_LDFLAGS = -Wl,-T./@ALLOCATOR_LDS_FILENAME@ -L@PKG_CONFIG_SYSROOT_DIR@/usr/lib -levi_lmm @axiom_libraries_LIBS@  
# @evidence_lmm_LIBS@ can not be used because it use a custom linker script (and we need to use onlu our allocator.lds)

#-L@PKG_CONFIG_SYSROOT_DIR@/usr/lib 

lib_LTLIBRARIES = libaxiom_allocator_l1.la libaxiom_allocator_l2.la libaxiom_allocator.la

libaxiom_allocator_la_SOURCES = axiom_allocator.c axiom_allocator_l3/axiom_allocator_l2_l3.c axiom_allocator_l3/axiom_allocator_l3_sw.c axiom_allocator_l3/axiom_allocator_l3_hw.c
#libaxiom_allocator_la_LIBADD = @AXIOM_RUN_LIB@/libaxiom_run_api.a ../../axiom-evi-nic/axiom_user_library/libaxiom_user_api.a @EVI_ALLOCATOR_LIB@/libevi_lmm.la
libaxiom_allocator_la_LIBADD = @axiom_libraries_LIBS@

libaxiom_allocator_l1_la_SOURCES = axiom_allocator_l1/axiom_allocator_l1.c

libaxiom_allocator_l2_la_SOURCES = axiom_allocator_l2/axiom_allocator_l2.c axiom_allocator_l2/axiom_allocator_l1_l2.c

include_HEADERS = $(top_srcdir)/include/axiom_allocator.h $(top_srcdir)/include/axiom_allocator_l2.h $(top_srcdir)/include/axiom_allocator_l1.h $(top_srcdir)/include/axiom_allocator_protocol.h
noinst_HEADERS = axiom_allocator_l2_l3.h axiom_allocator_l1_l2.h axiom_allocator_l3/axiom_allocator_l3.h
