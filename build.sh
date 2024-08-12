#!/bin/sh

set -x

echo $(gcc -Wall -Wextra -Wpedantic tlc.c -o tlc)
