ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/sort/mergesort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/sort/mergesort/rules.mk
#-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

# Sample linking rules for in-source build:
test_mergesort.exe: ${d}/mergesort.o ${d}/test_mergesort.o
