#AM_CFLAGS = -I$(top_srcdir)/include/ -I@AXIOM_EVI_NIC_INCLUDE@
AM_CFLAGS = -I$(top_srcdir)/include/ @axiom_libraries_CFLAGS@ @evidence_lmm_CFLAGS@
#AM_LDFLAGS = -Wl,-T@EVI_ALLOCATOR_LDS@ -rpath @EVI_ALLOCATOR_LIB@
AM_LDFLAGS = @axiom_libraries_LIBS@ @evidence_lmm_LIBS@ -L@PKG_CONFIG_SYSROOT_DIR@/usr/lib

if DEBUG
AM_CFLAGS += -DPDEBUG
endif

bin_PROGRAMS = axiom-allocator-test

axiom_allocator_test_SOURCES = axiom-allocator-test.c
axiom_allocator_test_LDADD = $(top_srcdir)/src/libaxiom_allocator.la

