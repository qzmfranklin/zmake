ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/hack/threads/mutex# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/hack/threads/mutex/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

static.exe: ${d}/static.o
dynamic.exe: ${d}/dynamic.o
switch.exe: ${d}/switch.o
pthread_whosfirst.exe: ${d}/pthread_whosfirst.o
