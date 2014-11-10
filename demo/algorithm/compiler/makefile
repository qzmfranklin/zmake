CXX:=gcc
CXXFLAGS:=-O3
LIBS:=-L/usr/local/Cellar/flex/2.5.37/lib -lfl

FLEX:=flex
BISON:=bison


all: calculator.exe wc.exe

clean:
	rm -f *.exe *.yy.c *.tab.c *.tab.h *.o *.d

calculator.exe: calculator.y calculator.l
	$(BISON) -d calculator.y
	$(FLEX) -o calculator.yy.c calculator.l
	$(CXX) -o $@ calculator.yy.c calculator.tab.c $(LIBS)

wc.exe: wc.l
	$(FLEX) -o wc.yy.c wc.l
	$(CXX) -o $@ wc.yy.c $(LIBS)
