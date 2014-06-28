ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/comp_geo# this directory
-include ${ROOT}/root.mk
#-include ${ROOT}/utils/rules.mk
-include ${ROOT}/comp_geo/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

# Sample linking rules for in-source build:
gen2dpts.exe: ${d}/gen2dpts.o

convex_hull.exe: ${d}/convex_hull.o
