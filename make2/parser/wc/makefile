CXX:=g++
CXXFLAGS:=-O3 -Wno-unused-result -Drestrict=__restrict__ \
	-DNDEBUG -pthread -fdiagnostics-color=always -std=c++11
FLEX:=flexc++
BISON=bisonc++

CPPFLAGS:=-isystem ../gtest/include -isystem ../gtest
LDFLAGS:=-pthread

EXE:=wc.exe

.PHONY: all clean test

all: ${EXE}

clean:
	rm -f *.o *.exe Scanner* Parser** lex.cc parse.cc

test: ${EXE}
	cp $< demo/$<
	cd demo
	./$<
	cd ..

wc.exe: wc.cc wc.l wc.y
	${BISON} wc.y
	${FLEX} wc.l
	echo "#include \"Parserbase.h\"" >> Scanner.ih
	${CXX} -c wc.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -c lex.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -c parse.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -o $@ *.o ${LDFLAGS}

%.o: %.cc
	${CXX} -c $< ${CXXFLAGS} ${CPPFLAGS}
