QUIET  :=@# comment this line to print more information 
ROOT    :=/media/MyData/codes/zmake/demo
d       :=${ROOT}# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/heapsort/rules.mk
-include ${DEP}
################################################################################
.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	${QUIET}rm -f *.exe ${OBJ} ${DEP} ${ASM}
################################################################################ 
#	LIST YOUR LINKING RULES HERE
test_heapsort.exe: ${d}/test_utils.o ${d}/utils.o
