ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/lulu/quant_diagram# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/lulu/quant_diagram/rules.mk
-include ${DEP}

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

permute.exe: ${d}/permute.o
