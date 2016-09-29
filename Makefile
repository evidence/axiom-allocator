PWD := $(shell pwd)

AXIOM_ALLOC_INCLUDE := ${PWD}/include
AXIOM_ALLOC_SRC := ${PWD}/src
AXIOM_ALLOC_TEST := ${PWD}/test

HEADERS := $(AXIOM_ALLOC_INCLUDE)/*.h $(AXIOM_ALLOC_SRC)/*.h
LIBS := $(AXIOM_ALLOC_SRC)/libaxiom_allocator.so
TESTS := $(AXIOM_ALLOC_TEST)/axiom-allocator-test
CLEANFILES = $(LIBS) $(TESTS) *.o */*.o

ifdef CCARCH
    BUILDROOT := ${PWD}/../axiom-evi-buildroot
    DESTDIR := ${BUILDROOT}/output/target
    CCPREFIX := ${BUILDROOT}/output/host/usr/bin/$(CCARCH)-linux-
endif

CC := ${CCPREFIX}gcc
AR := ${CCPREFIX}ar
RANLIB := ${CCPREFIX}ranlib

DFLAGS := -g -DPDEBUG
CFLAGS += -fPIC -Wall $(DFLAGS) -I$(AXIOM_ALLOC_INCLUDE)
LDFLAGS += -L$(AXIOM_ALLOC_SRC)
LDLIBS += -laxiom_allocator
#LDLIBS += -Wl,--whole-archive -laxiom_allocator -Wl,--no-whole-archive

.PHONY: all tests clean install

all: $(LIBS)

tests: $(LIBS) $(TESTS)

$(AXIOM_ALLOC_SRC)/libaxiom_allocator.so: $(AXIOM_ALLOC_SRC)/axiom_allocator.o $(AXIOM_ALLOC_SRC)/axiom_sw_l3_allocator.o $(AXIOM_ALLOC_SRC)/axiom_hw_l3_allocator.o $(AXIOM_ALLOC_SRC)/axiom_l2_allocator.o
	$(CC) ${LDFLAGS} -shared -o $@ $^

$(AXIOM_ALLOC_SRC)/axiom_allocator.o: $(AXIOM_ALLOC_SRC)/axiom_allocator.c $(HEADERS)

$(AXIOM_ALLOC_SRC)/axiom_sw_l3_allocator.o: $(AXIOM_ALLOC_SRC)/axiom_sw_l3_allocator.c $(HEADERS)

$(AXIOM_ALLOC_SRC)/axiom_hw_l3_allocator.o: $(AXIOM_ALLOC_SRC)/axiom_hw_l3_allocator.c $(HEADERS)

$(AXIOM_ALLOC_SRC)/axiom_l2_allocator.o: $(AXIOM_ALLOC_SRC)/axiom_l2_allocator.c $(HEADERS)

$(AXIOM_ALLOC_TEST)/axiom-allocator-test: $(AXIOM_ALLOC_TEST)/axiom-allocator-test.o

install: all
	cp $(TEST) $(DESTDIR)/usr/bin/

clean:
	rm -rf $(CLEANFILES)
