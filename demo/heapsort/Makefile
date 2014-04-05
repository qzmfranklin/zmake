#QUIET  :=@# comment this line to print more information 
ROOT    :=/media/MyData/codes/zmake/demo
d       :=${ROOT}/heapsort# this directory
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
# Sample linking rules for in-source build:
test_heapsort.exe: ${d}/heapsort.o ${d}/test_heapsort.o ${ROOT}/utils/utils.o
