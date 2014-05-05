ROOT    :=/media/qzmfranklin/data/git/zmake/demo
d       :=${ROOT}/sort/quicksort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/sort/quicksort/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_quicksort.exe: ${d}/quicksort.o ${d}/test_quicksort.o ${ROOT}/utils/utils.o
