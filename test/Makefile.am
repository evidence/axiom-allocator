AM_CFLAGS = -I$(top_srcdir)/include/

if DEBUG
AM_CFLAGS += -DPDEBUG
endif

bin_PROGRAMS = axiom-allocator-test

axiom_allocator_test_SOURCES = axiom-allocator-test.c
axiom_allocator_test_LDADD = $(top_srcdir)/src/libaxiom_allocator.la

