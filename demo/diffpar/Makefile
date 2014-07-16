ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/diffpar# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/diffpar/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

diffpar.exe: ${d}/diffpar.o
