ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/sort/heapsort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/sort/heapsort/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_heapsort.exe: ${d}/heapsort.o ${d}/test_heapsort.o ${ROOT}/utils/utils.o
