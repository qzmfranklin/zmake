ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/diffpar#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/diffpar/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

diffpar.exe: ${d}/diffpar.o
