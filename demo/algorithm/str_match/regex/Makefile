ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/str_match/regex# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/str_match/regex/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

simple_regex.exe: ${d}/simple_regex.o
