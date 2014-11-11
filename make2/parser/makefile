CXX:=g++-4.9
CXXFLAGS:=-O3 -Wshadow -Wno-unused-result -Drestrict=__restrict__ \
	-DNDEBUG -pthread -fdiagnostics-color=always -std=c++11
FLEX:=flexc++
BISON=bisonc++

CPPFLAGS:=-isystem ../gtest/include -isystem ../gtest
LDFLAGS:=-pthread

EXE:=wc.exe

.PHONY: all clean test

all: ${EXE}

clean:
	rm -f *.o *.exe

test: ${EXE}
	cp $< demo/$<
	cd demo
	./$<
	cd ..

wc.exe: wc.cc wc.lex
	${FLEX} wc.lex
	${CXX} -c wc.cc ${CXXFLAGS} ${CPPFLAGS}
	${CXX} -c wc_lex.cc ${CXXFLAGS} ${CPPFLAGS}
	${CXX} -o $@ wc.o wc_lex.o ${LDFLAGS}

lex.o: wc.lex
	flexc++

%.o: %.cc
	${CXX} -c $< ${CXXFLAGS} ${CPPFLAGS}
