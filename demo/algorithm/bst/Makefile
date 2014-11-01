ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/bst#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/bst/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_bst.exe: ${d}/bst.o ${d}/test_bst.o
