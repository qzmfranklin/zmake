ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/gtest# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/gtest/rules.mk
-include ${DEP}

.PHONY: all asm clean test
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

TESTS :=sample1_unittest.exe \
	sample2_unittest.exe \
	sample3_unittest.exe \
	sample4_unittest.exe \
	sample5_unittest.exe \
	sample6_unittest.exe \
	sample7_unittest.exe \
	sample8_unittest.exe \
	sample9_unittest.exe \
	sample10_unittest.exe

sample1_unittest.exe:  ${d}/samples/sample1.o ${d}/samples/sample1_unittest.o ${d}/gtest_main.a
sample2_unittest.exe:  ${d}/samples/sample2.o ${d}/samples/sample2_unittest.o ${d}/gtest_main.a
sample3_unittest.exe:  ${d}/samples/sample3_unittest.o ${d}/gtest_main.a
sample4_unittest.exe:  ${d}/samples/sample4.o ${d}/samples/sample4_unittest.o ${d}/gtest_main.a
sample5_unittest.exe:  ${d}/samples/sample1.o ${d}/samples/sample5_unittest.o ${d}/gtest_main.a
sample6_unittest.exe:  ${d}/samples/sample6_unittest.o ${d}/gtest_main.a
sample7_unittest.exe:  ${d}/samples/sample7_unittest.o ${d}/gtest_main.a
sample8_unittest.exe:  ${d}/samples/sample8_unittest.o ${d}/gtest_main.a
sample9_unittest.exe:  ${d}/samples/sample9_unittest.o ${d}/gtest_main.a
sample10_unittest.exe: ${d}/samples/sample10_unittest.o ${d}/gtest_main.a

test: $(TESTS)
	for t in $^ ; do ./"$$t" ; done
