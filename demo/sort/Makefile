ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/sort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/sort/rules.mk
-include ${ROOT}/sort/heapsort/rules.mk
-include ${ROOT}/sort/quicksort/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

cmp_sort.exe: ${d}/cmp_sort.o \
	${d}/heapsort/heapsort.o \
	${d}/quicksort/quicksort.o \
	${ROOT}/utils/utils.o \
	${ROOT}/utils/Table.o
