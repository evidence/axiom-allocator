AM_CFLAGS = -Wall -I$(top_srcdir)/include

if DEBUG
AM_CFLAGS += -DPDEBUG
endif

lib_LTLIBRARIES = libaxiom_allocator.la
libaxiom_allocator_la_SOURCES = axiom_allocator.c axiom_l2_allocator.c axiom_sw_l3_allocator.c axiom_hw_l3_allocator.c
include_HEADERS = $(top_srcdir)/include/axiom_allocator.h $(top_srcdir)/include/dprintf.h
noinst_HEADERS = axiom_l2_allocator.h axiom_l3_allocator.h
