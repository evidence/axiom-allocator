#!/bin/sh
SCRIPT_PWD=$(dirname `which $0`)

set -x
LD_LIBRARY_PATH=${SCRIPT_PWD}/src:$LD_LIBRARY_PATH $@
