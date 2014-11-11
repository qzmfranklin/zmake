CXX:=g++-4.9
CXXFLAGS:=-O3 -Wshadow -Wno-unused-result -Drestrict=__restrict__ \
	-DNDEBUG -pthread -fdiagnostics-color=always -std=c++11

CPPFLAGS:=-isystem ../gtest/include -isystem ../gtest
LDFLAGS:=-pthread

EXE:=gtest_dag.exe

.PHONY: all clean test

all: ${EXE}

clean:
	rm -f *.o *.exe

test: ${EXE}
	cp $< demo/$<
	cd demo
	./$<
	cd ..

${EXE}: dag.o gtest_dag.o ../gtest/gtest_main.a
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} -o $@ -c ${CXXFLAGS} ${CPPFLAGS} $<
