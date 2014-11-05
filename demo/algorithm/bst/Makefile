ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/bst# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/bst/rules.mk
-include ${ROOT}/gtest/rules.mk
-include ${DEP}

TESTS:=gtest_bst.exe

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}
test: ${TESTS}
	for t in $^ ; do ./"$$t" ; done

gtest_bst.exe: ${d}/gtest_bst.o ${d}/bst.o ${ROOT}/gtest/gtest_main.a
