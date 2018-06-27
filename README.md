# AXIOM 3-level Memory Allocator API

----

**NOTE: This repository is a submodule of axiom-evi project. Please clone the
master project from https://github.com/evidence/axiom-evi**

This repository contains the implementation of the AXIOM 3-level Memory
Allocator API.
The code is organized with the following directories:

 * ./include
    + contains the API exposed to the User applications
 * ./src
    + contains the implementation of the Axiom Allocator API
 * ./test
    + contains some tests of the API


## How to compile

```
# Use the axiom-evi/scripts/Makefile to compile the AXIOM Allocator API
cd axiom-evi/scripts

# Configure all modules (required only the first time)
make MODE=aarch64 configure

# compile the libaxiom_allocator.so and tests
make allocator
```

## How to run tests
```
test/axiom-allocator-test

```

