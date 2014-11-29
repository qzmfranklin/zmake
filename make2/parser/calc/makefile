CXX:=g++-4.9
CXXFLAGS:=-O3 -Wno-unused-result -fdiagnostics-color=always -std=c++11
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

gen: calc.l calc.y
	${FLEX} calc.l
	${BISON} calc.y
	echo "#include \"Parserbase.h\"" >> Scanner.ih

calc.exe: calc.cc lex.cc parse.cc
	${CXX} -c calc.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -c lex.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -c parse.cc ${CPPFLAGS} ${CXXFLAGS}
	${CXX} -o $@ *.o ${LDFLAGS}

%.o: %.cc
	${CXX} -c $< ${CXXFLAGS} ${CPPFLAGS}
