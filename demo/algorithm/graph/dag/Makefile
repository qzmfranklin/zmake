ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/graph/dag#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/graph/dag/rules.mk
-include ${ROOT}/gtest/rules.mk
-include ${DEP}

TESTS:=gtest_dag.exe

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}
test: ${TESTS}
	for t in $^ ; do ./"$$t" ; done

gtest_dag.exe: ${d}/gtest_dag.o ${d}/dag.o ${ROOT}/gtest/gtest_main.a
