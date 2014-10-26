ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/sort/mergesort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/sort/mergesort/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_mergesort.exe: ${d}/mergesort.o ${d}/test_mergesort.o
