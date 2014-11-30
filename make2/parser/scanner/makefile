TESTS:=test_scanner.exe

CXX:=g++ -fdiagnostics-color=always -std=c++11 -Drestrict=__restrict__
CXXFLAGS:=-O3 -Wno-unused-result -DNDEBUG -pthread
LDFLAGS:=-pthread

FLEX:=flexc++

clean:
	rm -f lex.cc Scanner.ih *.o parse.cc Parser.ih *.exe

test_scanner.exe: test_scanner.o lex.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) -c $< $(CXXFLAGS)

lex.o: lex.cc Scanner.h Scanner.ih Scannerbase.h

lex.cc Scanner.h Scanner.ih Scannerbase.h: make2.l
	$(FLEX) $<
