ROOT    :=/Users/qzmfrank/git/zmake/demo
d       :=${ROOT}/algorithm/hough_trans# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/algorithm/hough_trans/rules.mk
#-include ${ROOT}/utils/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}
