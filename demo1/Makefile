CXX:=icpc


CFLAGS1:=-O3
CFLAGS2:=-g
INCS:=-iquote .

test_heapsort.exe: test_heapsort.o heapsort.o utils.o
	${CXX} -o $@ $^ -lm

test_utils.exe: test_utils.o utils.o
	${CXX} -o $@ $^

test_heapsort.o: heapsort/test_heapsort.cpp \
			heapsort/heapsort.h utils/utils.h 
	${CXX} -o $@ -c $< ${CFLAGS1} ${INCS}

heapsort.o: heapsort/heapsort.cpp heapsort/heapsort.h
	${CXX} -o $@ -c $< ${CFLAGS1} ${INCS} 
	
test_utils.o: utils/test_utils.cpp utils/utils.h
	${CXX} -o $@ -c $< ${CFLAGS2} ${INCS}

utils.o: utils/utils.cpp utils/utils.h
	${CXX} -o $@ -c $< ${CFLAGS2} ${INCS}

.PHONY: all clean cleanxx

all: 

clean:
	rm -f *.o
cleanxx: clean
	rm -f *.exe
