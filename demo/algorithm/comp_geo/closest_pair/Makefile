<<<<<<< HEAD:demo/comp_geo/closest_pair/Makefile
ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/comp_geo/closest_pair# this directory
=======
ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/comp_geo/closest_pair# this directory
>>>>>>> 47d2514d20df243a6a77623c64a195d9e2e5e591:demo/algorithm/comp_geo/closest_pair/Makefile
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/comp_geo/rules.mk
-include ${ROOT}/algorithm/comp_geo/closest_pair/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_closest_pair.exe: ${d}/test_closest_pair.o  ${d}/closest_pair.o \
	${ROOT}/algorithm/comp_geo/geo_utils.o

time_closest_pair.exe: ${d}/time_closest_pair.o  ${d}/closest_pair.o \
	${ROOT}/algorithm/comp_geo/geo_utils.o ${ROOT}/utils/Table.o
