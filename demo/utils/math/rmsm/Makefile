ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/utils/math/rmsm# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/utils/math/rmsm/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

test_rmsm.exe: ${d}/rmsm.o ${d}/test_rmsm.o

time_rmsm.exe: ${d}/rmsm.o ${d}/time_rmsm.o ${ROOT}/utils/Table.o

time_rmsm_large.exe: ${d}/rmsm.o ${d}/time_rmsm_large.o \
	${ROOT}/utils/Table.o
