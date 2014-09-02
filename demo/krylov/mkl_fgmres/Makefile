ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/krylov/mkl_fgmres# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/krylov/mkl_fgmres/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_mkl_solvers.exe: ${d}/test_mkl_solvers.o
