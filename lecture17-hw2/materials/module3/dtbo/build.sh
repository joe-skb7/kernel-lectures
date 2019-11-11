#!/bin/sh

set -e

in=hw3.dtso
gen=hw3_gen.dtso
out=hw3.dtbo
kernel_dir=~/repos/linux-stable
gcc_flags="-E -P -x assembler-with-cpp -I$kernel_dir/include"
dtc_flags="-W no-unit_address_vs_reg -I dts -O dtb -b 0 -@"

rm -f $out
gcc $gcc_flags -o $gen $in
dtc $dtc_flags -o $out $gen
rm -f $gen
