ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/comp_geo/farthest_pair#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/comp_geo/rules.mk
-include ${ROOT}/algorithm/comp_geo/farthest_pair/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

graham_scan.exe: ${d}/graham_scan.o ${d}/convex_hull.o ${ROOT}/algorithm/comp_geo/geo_utils.o

test_convex_hull.exe: ${d}/test_convex_hull.o ${d}/convex_hull.o \
	${ROOT}/algorithm/comp_geo/geo_utils.o
