LEMON:=lemon
CXX:=g++
CXXFLAG:=-O3

EXE:=ex1.exe

.SUFFIXES:


clean:
	rm -f *.o ex1.cpp ex1.c ex1.h ex1.out $(EXE)

all: $(EXE)


test: all
	./ex1.exe

ex1.exe: ex1.o main.o

ex1.c ex1.h: ex1.y
	$(LEMON) $<

%.exe: %.o
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) -c $<

%.o: %.c
	$(CXX) -c $<
