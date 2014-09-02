<<<<<<< HEAD
ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/str_match/regex# this directory
=======
ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/str_match/regex# this directory
>>>>>>> 47d2514d20df243a6a77623c64a195d9e2e5e591
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/str_match/regex/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

simple_regex.exe: ${d}/simple_regex.o
