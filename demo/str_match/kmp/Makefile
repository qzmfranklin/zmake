ROOT    :=/Users/qzmfrank/codes/zmake/demo
d       :=${ROOT}/str_match/kmp# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/str_match/kmp/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

kmp_match.exe: ${d}/kmp_match.o
