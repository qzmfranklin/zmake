ROOT    :=/Users/qzmfrank/codes/zmake/demo
d       :=${ROOT}/comp_geo/closest_pair# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/comp_geo/rules.mk
-include ${ROOT}/comp_geo/closest_pair/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

closest_pair.exe: ${d}/closest_pair.o ${ROOT}/comp_geo/geo_utils.o
