ROOT    :=/Users/qzmfrank/codes/zmake/demo
d       :=${ROOT}/str_match/regex# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/comp_geo/rules.mk
-include ${ROOT}/comp_geo/closest_pair/rules.mk
-include ${ROOT}/comp_geo/farthest_pair/rules.mk
-include ${ROOT}/sort/rules.mk
-include ${ROOT}/sort/heapsort/rules.mk
-include ${ROOT}/sort/mergesort/rules.mk
-include ${ROOT}/sort/quicksort/rules.mk
-include ${ROOT}/str_match/kmp/rules.mk
-include ${ROOT}/str_match/regex/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

# Sample linking rules for in-source build:
#test_heapsort.exe: ${d}/heapsort.o ${d}/test_heapsort.o ${ROOT}/utils/utils.o
