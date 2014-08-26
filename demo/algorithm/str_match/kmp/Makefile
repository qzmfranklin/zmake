<<<<<<< HEAD
ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/str_match/kmp# this directory
=======
ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/str_match/kmp# this directory
>>>>>>> 47d2514d20df243a6a77623c64a195d9e2e5e591
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/str_match/kmp/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

kmp_match.exe: ${d}/kmp_match.o
