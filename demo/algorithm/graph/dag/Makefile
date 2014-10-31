ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/graph/dag#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/graph/dag/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_dag.exe: ${d}/test_dag.o ${d}/dag.o
