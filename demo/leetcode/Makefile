ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/leetcode# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/leetcode/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

hist_max_area.exe: ${d}/hist_max_area.o
