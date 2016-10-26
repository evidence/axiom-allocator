AM_CFLAGS = -I$(top_srcdir)/include/
AM_LDFLAGS = -Wl,-T@EVI_ALLOCATOR_LDS@

if DEBUG
AM_CFLAGS += -DPDEBUG
endif

bin_PROGRAMS = axiom-allocator-test

axiom_allocator_test_SOURCES = axiom-allocator-test.c
axiom_allocator_test_LDADD = $(top_srcdir)/src/libaxiom_allocator.la

