ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/hack/asm# this directory
-include ${ROOT}/root.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

%.exe: %.s
	${CC} -o $@ $< ${CFLAGS} -g

addmany.exe: ${d}/addmany.s
