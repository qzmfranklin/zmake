ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/heap# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/heap/rules.mk
-include ${DEP}

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_heap.exe: ${d}/heap.o ${d}/test_heap.o
