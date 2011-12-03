#!/bin/sh
# reference 
# http://objectmix.com/tcl/15449-how-cross-compile-tcl8-4-tk8-4-arm-linux.html
export CROSS_COMPILE=arm-linux
export CC=arm-linux-gcc
export ac_cv_func_strtod=yes
export tcl_cv_strtod_buggy=1
./configure --prefix=$PUBLIC/tcltest \
--target=arm-linux \
--host=arm-linux \
--build=i386-pc-linux \
--with-endianness=little \
--with-cc=arm-linux-gcc \
--with-ar=arm-linux-ar \
--with-install-prefix=../.. \
--with-cflags="-O0 -msoft-float -march=armv4t -mtune=arm920t -mcpu=arm920t"

