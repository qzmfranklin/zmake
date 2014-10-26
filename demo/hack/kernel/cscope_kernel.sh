#!/bin/sh
LNX=$1
find  $LNX                                                                \
	-path "$LNX/arch/*" ! -path "$LNX/arch/i386*" -prune -o               \
	-path "$LNX/include/asm-*" ! -path "$LNX/include/asm-i386*" -prune -o \
	-path "$LNX/tmp*" -prune -o                                           \
	-path "$LNX/Documentation*" -prune -o                                 \
	-path "$LNX/scripts*" -prune -o                                       \
	-path "$LNX/drivers*" -prune -o                                       \
	-name "*.[chxsS]" -print > cscope.files

cscope -b -q -k
