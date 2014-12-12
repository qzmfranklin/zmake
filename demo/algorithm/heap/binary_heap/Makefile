ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/heap/binary_heap# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/heap/binary_heap/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_binary_heap.exe: ${d}/test_binary_heap.o
