AM_CFLAGS = -Wall -I$(top_srcdir)/include

if DEBUG
AM_CFLAGS += -DPDEBUG
endif

lib_LTLIBRARIES = libaxiom_allocator.la
libaxiom_allocator_la_SOURCES = axiom_allocator.c axiom_allocator_l2/axiom_allocator_l2_l3.c axiom_allocator_l3/axiom_allocator_l3_sw.c axiom_allocator_l3/axiom_allocator_l3_hw.c
include_HEADERS = $(top_srcdir)/include/axiom_allocator.h $(top_srcdir)/include/dprintf.h
noinst_HEADERS = axiom_allocator_l2_l3.h axiom_allocator_l3/axiom_allocator_l3.h
