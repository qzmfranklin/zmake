ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/network/tcpip# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/network/tcpip/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

showip.exe: ${d}/showip.o ${d}/inet_utils.o
