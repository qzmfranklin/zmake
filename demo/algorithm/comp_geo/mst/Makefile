ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/comp_geo/mst#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/comp_geo/rules.mk
-include ${ROOT}/algorithm/comp_geo/mst/rules.mk
-include ${ROOT}/algorithm/heap/fib_heap/rules.mk
-include ${ROOT}/gtest/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

mst.exe: ${d}/mst.o ${ROOT}/algorithm/comp_geo/geo_utils.o
