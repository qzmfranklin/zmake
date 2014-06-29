ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/comp_geo/farthest_pair# this directory
-include ${ROOT}/root.mk
#-include ${ROOT}/utils/rules.mk
-include ${ROOT}/comp_geo/rules.mk
-include ${ROOT}/comp_geo/farthest_pair/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

# Sample linking rules for in-source build:
gen2dpts.exe: ${d}/gen2dpts.o

graham_scan.exe: ${d}/graham_scan.o ${d}/convex_hull.o ${ROOT}/comp_geo/geo_utils.o

test_convex_hull.exe: ${d}/test_convex_hull.o ${d}/convex_hull.o \
	${ROOT}/comp_geo/geo_utils.o
