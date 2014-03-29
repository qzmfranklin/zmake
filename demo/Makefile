#  Sample Makefile
################################################################################
#	ROOT LEVEL INFO - MAKEFILE SPECIFIC
#OUT_OF_SOURCE:=TRUE# TRUE = out-of-source build, otherwise = in-source build
#BUILD:=.# out-of-source build directory, unused in in-source build mode
# Strongly recommend only using ${BUILD}=. as it simplifies the linking rules
#QUIET:=@# uncomment this line to silence
ROOT :=/Users/qzmfrank/codes/zmake/demo
# Include the top-level .mk file, i.e., the root.mk
-include ${ROOT}/root.mk
# Include all branches here
-include ${ROOT}/heapsort/rules.mk
-include ${ROOT}/utils/rules.mk
# Then include all dependency files
-include ${DEP}
################################################################################ 
#	LIST YOUR LINKING RULES HERE

# Sample linking rules for out-of-source build:
# If ${BUILD}==. (current directory), then we can use the following linking rule
#diffpar.exe: diffpar.o utils.o
#test_heapsort.exe: heapsort.o test_heapsort.o utils.o

# If ${BUILD} is anything else, we have to use the following linking rule
#${BUILD}/diffpar.exe: ${BUILD}/diffpar.o ${BUILD}/utils.o
#${BUILD}/test_heapsort.exe: ${BUILD}/heapsort.o ${BUILD}/test_heapsort.o ${BUILD}/utils.o


# Sample linking rules for in-source build:
#test_heapsort.exe: ${ROOT}/heapsort/heapsort.o \
	#${ROOT}/heapsort/test_heapsort.o \
	#${ROOT}/utils/utils.o
################################################################################
#	MISC PHONY TARGETS 
.PHONY: all asm clean cleanx cleanxx

all: ${OBJ}

asm: ${ASM}

clean:
	${QUIET}rm -f ${OBJ}
cleanx:
	${QUIET}rm -f ${DEP} ${ASM}
cleanxx: clean cleanx
	${QUIET}rm -f *.exe
