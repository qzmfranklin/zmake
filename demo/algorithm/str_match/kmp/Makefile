ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/str_match/kmp# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/str_match/kmp/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

kmp_match.exe: ${d}/kmp_match.o
