LEMON:=./lemon.exe
CFUNCTIONS:=cfunctions
CXX:=g++
CXXFLAG:=-O3 -std=c++11 -pedantic

EXE:=lemon.exe ex1.exe

.SUFFIXES:


all: $(EXE)

clean:
	rm -f *.o ex1.cpp ex1.c ex1.h ex1.hh ex1.out $(EXE)

test: all
	./ex1.exe

ex1.exe: ex1.o main.o ex1.hh

$(LEMON): lemon.o

ex1.c ex1.h ex1.hh:  ex1.y $(LEMON)
	$(LEMON) $<
	$(CFUNCTIONS) ex1.c -w ex1.c >> ex1.hh

%.exe: %.o
	$(CXX) -o $@ $^ $(CXXFLAG)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAG)

%.o: %.c
	$(CXX) -c $< $(CXXFLAG)
