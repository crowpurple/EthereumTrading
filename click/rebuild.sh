#!/bin/bash

export CURRENT_DIR=$(pwd)
# find core number 
cpuNumer=$(lscpu | awk -F ':' '{if(NR==4){print$2}}' | sed -e 's/^[ /t]*//')

#------------------------------------------------------------------------------
# This script rebuilds the clickos, if any changes
# are made on *.hh files
#------------------------------------------------------------------------------

# set MINIOS_ROOT 
export MINIOS_ROOT=$CURRENT_DIR"/../xen-4.4.1/extras/mini-os/"
echo "MINIOS_ROOT="$MINIOS_ROOT

# set XEN_ROOT
export XEN_ROOT=$CURRENT_DIR"/../xen-4.4.1/"
echo "XEN_ROOT="$XEN_ROOT

# set NEWLIB_ROOT
export NEWLIB_ROOT=$CURRENT_DIR"/../toolchain/x86_64-root/x86_64-xen-elf/"
echo "NEWLIB_ROOT="$NEWLIB_ROOT

# set LWIP_ROOT
export LWIP_ROOT=$CURRENT_DIR"/../toolchain/x86_64-root/x86_64-xen-elf/"
echo "LWIP_ROOT="$LWIP_ROOT

echo "========================================================================="
echo "Environment variable set up successfully."
echo "========================================================================="
sleep 1

# make clean if applicable. 
make clean
# ./configure
./configure --enable-minios --with-xen=$XEN_ROOT --with-minios=$MINIOS_ROOT

echo "========================================================================="
echo "Configuration successfully."
echo "========================================================================="
sleep 1

# make 
make -j $cpuNumer minios


