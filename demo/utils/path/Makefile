ROOT    :=/Volumes/SourceCode/git/zmake/demo
d       :=${ROOT}/utils/path# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/gtest/rules.mk
-include ${ROOT}/utils/path/rules.mk
-include ${DEP}

# List all the executables you want to run when you type `make test` in ${TESTS}
# Note that you need to -include ${ROOT}/gtest/rules.mk to actually build those
# executables
TESTS:=

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}
test: ${TESTS}
	for t in $^ ; do ./"$$t" ; done

# Sample linking rules for building executables:
#test_heapsort.exe: ${d}/heapsort.o ${d}/test_heapsort.o ${ROOT}/utils/utils.o
gtest_path.exe: ${d}/gtest_path.o ${d}/path.o ${ROOT}/gtest/gtest_main.a
