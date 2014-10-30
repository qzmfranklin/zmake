ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/hack/kernel/proc# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/hack/kernel/proc/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

address_space.exe: ${d}/address_space.o
