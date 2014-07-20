ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/network/tcpip/chat# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/network/tcpip/rules.mk
-include ${ROOT}/network/tcpip/chat/rules.mk
-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

client.exe: ${d}/client.o ${ROOT}/network/tcpip/inet_utils.o

server.exe: ${d}/server.o ${ROOT}/network/tcpip/inet_utils.o
