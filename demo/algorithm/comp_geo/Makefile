ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/comp_geo#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/comp_geo/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

gen2dpts.exe: ${d}/gen2dpts.o
