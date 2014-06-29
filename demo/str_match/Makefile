ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/str_match# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/str_match/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

# Sample linking rules for in-source build:
kmp_match.exe: ${d}/kmp_match.o
