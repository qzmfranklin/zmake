ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/graph/graphviz# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/graph/graphviz/rules.mk
-include ${DEP}

TESTS:=read_write_sample.exe dot.exe

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}
test: ${TESTS}
	./read_write_sample.exe < ex1.gv
	./dot.exe < ex1.gv

read_write_sample.exe: ${d}/read_write_sample.o
dot.exe: ${d}/dot.o
