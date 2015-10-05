ROOT    :=$(realpath ../..)
D       :=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))# this directory
-include $(ROOT)/root.mk
-include $(D)/rules.mk
-include $(DEP)

.PHONY: all asm clean test
all: $(OBJ)
asm: $(ASM)
clean:
	$(QUIET)rm -f *.exe $(OBJ) $(DEP) $(ASM)
test: endian.exe
	$(QUIET)./$<

endian.exe: $(D)/endian.o
