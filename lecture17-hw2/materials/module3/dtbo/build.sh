#!/bin/sh

set -e

input=hw3.dtso
gen=hw3_gen.dtso
output=hw3.dtbo
kernel_dir=~/repos/linux-stable
gcc_flags="-E -P -x assembler-with-cpp -I$kernel_dir/include -o $gen"
dtc_flags="-W no-unit_address_vs_reg -I dts -O dtb -o $output -b 0 -@"

rm -f $output
gcc $gcc_flags $input
dtc $dtc_flags $gen
rm -f $gen
