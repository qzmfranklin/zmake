ROOT    :=/home/qzmfrank/git/zmake/demo
d       :=${ROOT}/leetcode#this directory
-include ${ROOT}/root.mk
-include ${ROOT}/leetcode/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

hist_max_area.exe: ${d}/hist_max_area.o
max_rectangle.exe: ${d}/max_rectangle.o
max_path_sum.exe: ${d}/max_path_sum.o
trap_rain.exe: ${d}/trap_rain.o
sudoku_solver.exe: ${d}/sudoku_solver.o
