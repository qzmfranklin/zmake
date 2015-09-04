ROOT    :=$(realpath ..)
d       :=${ROOT}/utils# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_stat.exe: ${d}/test_stat.o

test_timestat.exe: ${d}/test_timestat.o

test_table.exe: ${d}/test_table.o ${d}/Table.o
