CXX:=g++-4.9
CXXFLAGS:=-O3 -Wshadow -Wno-unused-result -Drestrict=__restrict__ \
	-DNDEBUG -pthread -fdiagnostics-color=always -std=c++11
FLEX:=flexc++
BISON=bisonc++

CPPFLAGS:=
LDFLAGS:=

EXE:=calc.exe

.PHONY: all clean test

all: ${EXE}

clean:
	rm -f *.o *.exe Scanner* Parser** lex.cc parse.cc

test: ${EXE}
	cp $< demo/$<
	cd demo
	./$<
	cd ..

calc.exe: calc.cc calc.l calc.y
	${BISON} calc.y
	${FLEX} calc.l
	echo "#include \"Parserbase.h\"" >> Scanner.ih
	${CXX} -c calc.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -c lex.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -c parse.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -o $@ *.o ${LDFLAGS}

%.o: %.cc
	${CXX} -c $< ${CXXFLAGS} ${CPPFLAGS}
