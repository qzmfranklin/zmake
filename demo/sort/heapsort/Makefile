ROOT    :=/Users/qzmfrank/codes/zmake/demo
d       :=${ROOT}/sort/heapsort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/magic3/rules.mk
-include ${ROOT}/mem/latency/rules.mk
-include ${ROOT}/mem/bw/rules.mk
-include ${ROOT}/cpu/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/sort/heapsort/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_heapsort.exe: ${d}/heapsort.o ${d}/test_heapsort.o ${ROOT}/utils/utils.o
