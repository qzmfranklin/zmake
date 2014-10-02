ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/bst# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/bst/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

# Sample linking rules for in-source build:
#test_heapsort.exe: ${d}/heapsort.o ${d}/test_heapsort.o ${ROOT}/utils/utils.o
