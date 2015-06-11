ROOT    :=$(abspath ../..)
d       :=$(ROOT)/algorithm/diffpar# this directory
-include $(ROOT)/root.mk
-include $(ROOT)/algorithm/diffpar/rules.mk
-include $(DEP)

.PHONY: all asm clean test
all: $(OBJ)
asm: $(ASM)
clean:
	$(QUIET)rm -f *.exe $(OBJ) $(DEP) $(ASM)
	$(QUIET)echo "Cleaning ..."

diffpar.exe: ${d}/diffpar.o
