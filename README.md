zmake
=====
A new method of writing Makefiles.

# A Brief History of Makefiles

## Sample Project

There is a `demo/` directory in this repo. The source tree is:

```
demo/
├── sort/
│   ├── cmp_sort.cpp
│   ├── cmp_sort.exe
│   ├── heapsort/
│   │   ├── heapsort.cpp
│   │   ├── heapsort.h
│   │   ├── test_heapsort.cpp
│   │   └── test_heapsort.exe
│   ├── quicksort/
│   │   ├── quicksort.cpp
│   │   ├── quicksort.h
│   │   ├── test_quicksort.cpp
│   │   └── test_quicksort.exe
└── utils/
    ├── StatVector.h
    ├── Table.cpp
    ├── Table.h
    ├── test_stat.cpp
    ├── test_stat.exe
    ├── test_table.cpp
    ├── test_table.exe
    ├── test_timestat.cpp
    ├── test_timestat.exe
    ├── TimeStamp.h
    ├── TimeStat.h
    ├── utils.cpp
    └── utils.h

```

The .cpp files are the C++ source files.

The .h files are the C/C++ header files.

The .exe files are the executables *to build*.

A questions arises naturally: how to build the executables?

## The make Program

The standard way of building executables from a source tree like this is to use the make program. The make program takes a file, by default called Makefile, from the commandline.

The Makefile provides information on the dependencies in the form of `target: prerequisite(s)`. There is usually also a `recipe` associated with each target. If the target is outdated, that is, older than any of its prerequisites, the make program will invoke the recipe associated with the target in an attempt to rebuild the target.

The dependencies in a project will form a *directed acyclic graph* (DAG). The make program parses the DAG, determines which recipes to invoke, and invokes the recipes in a proper order.

The reader is assumed to be familiar with tagets, prerequisites, recipes, dependencies, Unix shell commands, and compiler options.

## Several Important Methods

In principle, one can list dependencies by hand in a *monolithic* Makefile. In practice, nobody cares to do that because

> As the project grows larger, it would at some point become practically impossible to read, modify, and maintain the monolithic Makefile.

Pattern rules such as `.o .c` and `%.o : %.c` can save some work. But such pattern rules do not permit ***directory-specific*** compiling and linking options, which are the keys to achieving ***directory level modularity***.

The method of *recursive make* can easily achieve the directory level modularity. But, as Peter Miller pointed out in his famous [*Recursive make considered harmful*](http://aegis.sourceforge.net/auug97.pdf), recursive make breaks the completeness of the DAG. This has several consequencies:

* The incompleteness of the DAG means that the make program will usually build too little.

* Manually "patching" the DAG would usually result in an over-complete Makefile that builds too much.

* Parallel make using the `make -j` becomes very tricky, if at all possible.

Peter Miller concluded that 
> the right way of writing Makefiles was to write a single Makefile the could build the entire project.

Peter Miller proposed an *alternative single Makefile solution* (section 7 'Big Picture' in his paper). His solution had a file called `modules.mk` in each subdirectory to store directory specific information, and had the Makefile in the root directory include all the `modules.mk` files. However, his solution did not fully support directory level modularity because it passed compiling options using a global variable. 

Emile van Bergen, was aware of these, and proposed a [new solution](http://evbergen.home.xs4all.nl/nonrecursive-make.html).

Similar to Peter's approach, Emile's solution was to write `Rules.mk` files for each subdirectory and have the Makefile in the root directory inlcude all the `Rules.mk` in the right order.

Emile's solution used stacks and stack pointers to keep track of paths. As the make program enters a `Rules.mk` (assuming `Rules.mk` is in a subdirectory called `subdir`), the parent directory of `subdir` is pushed into a stack and the stack pointer shifts to `subdir`. As the make program leaves `subdir`, the parent directory is poped from the stack and the stack pointer shifts back to the parent directory.

The advantages of Emile's solution are:

* Each `Rules.mk` is oblivious of any other `Rules.mk`'s.
* In principle, Emile's solution allows programmers to specify recipes for each *target*.

The disadvantages of Emile's solutions are:

* The correctness of the `Makefile` depends on the order in which the `Rules.mk` files are included.
* Recipes for each target sounds great. But in most situations it is an overkill and means that the programmers still have to list recipes by hand.


# Yet Another Method of Writing Makefiles

## Introduction to Modules
The above discussion on the history of Makefiles lead us to think of a method of writing a single Makefile that supports ***directory-specific pattern rules***.

Similar to Peter's and Emile's methods, my solution has a `rules.mk` file in each subdirectory. Take a look at `demo/sort/quicksort/rules.mk` as an example:

```
#  THIS DIRECTORY
DIR00006:=${ROOT}/sort/quicksort
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00006}C:=
${DIR00006}CPP:=quicksort.cpp test_quicksort.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00006}CFLAGS:=${CFLAGS}
${DIR00006}CXXFLAGS:=${CXXFLAGS}
${DIR00006}INCS:=${INCS}
${DIR00006}LIBS:=${LIBS}

DEP+=${${DIR00006}CPP:%.cpp=${DIR00006}/%.d} ${${DIR00006}C:%.c=${DIR00006}/%.d} 
OBJ+=${${DIR00006}CPP:%.cpp=${DIR00006}/%.o} ${${DIR00006}C:%.c=${DIR00006}/%.o} 
ASM+=${${DIR00006}CPP:%.cpp=${DIR00006}/%.s} ${${DIR00006}C:%.c=${DIR00006}/%.s} 

${DIR00006}/%.o: ${DIR00006}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00006}CFLAGS} ${${DIR00006}INCS}
${DIR00006}/%.s: ${DIR00006}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00006}CFLAGS} ${${DIR00006}INCS}

${DIR00006}/%.o: ${DIR00006}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00006}CXXFLAGS} ${${DIR00006}INCS}
${DIR00006}/%.s: ${DIR00006}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00006}CXXFLAGS} ${${DIR00006}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00006}/%.o
	${CXX} -o $@ $^ ${${DIR00006}LIBS}
```

* `ROOT` stores the *absolute path* of the root of the source tree. `ROOT` is defined in Makefiles as a global variable, e.g., `ROOT:=~/git/zmake/demo`.

* `DIRXXXXX` (in this case, `XXXXX` is `00006`) stores the *absolute path* of this subdirectory (in this case, `${ROOT}/sort/quicksort`). Each subdirectory is given a unique number.

* `${DIRXXXXX}CFLAGS/CXXFLAGS/INCS/LIBS` store the C compiling options, C++ compiling options, include directories, and the linking options for all recipes in this subdirectory, respectively. Because `DIRXXXXX` is unique, all four variables are unique across different `rules.mk` files.

* `CXX` is the C++ compiler. It is defined in `demo/root.mk`: `CXX:=g++`.

* `DEPFlAGS` is defined in `demo/root.mk`: `DEPFLAGS:=-MMD -MP`. It instructs the compiler to generate .d files.

* `ASMFLAGS` is defined in `demo/root.mk`: `ASMFLAGS:=-S`. It instructs the compiler to generate assembly listing files.


The core of this method lies in the pattern rules such as the following:

```
${DIR00006}/%.o: ${DIR00006}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00006}CXXFLAGS} ${${DIR00006}INCS}
```
The above pattern rule applies to
```
${ROOT}/sort/quicksort/*.o: ${ROOT}/sort/quicksort/*.cpp
```
But it does **not** applyt to targets in other directories. For example, it does not apply to
```
${ROOT}/sort/cmp_sort.o: ${ROOT}/sort/cmp_sort.cpp
```
Because `${${DIXXXXX}CXXFLAGS}` and `${${DIRXXXXX}INCS}` are unique to each subdirectory, the pattern rules defined in `rules.mk` only apply to their own subdirectory.

The `demo/root.mk` mentioned before is:
```
#	PROJECT-WIDE COMMON COMPILING FLAGS 
CC		:=g++
CFLAGS 		:=-O3 -Wall -Drestrict=__restrict__

CXX		:=${CC}
CXXFLAGS	:=${CFLAGS}

#       PROJECT-WIDE DEFAULT LINKING LIBRARIES AND INCLUDE DIRECTORIES
INCS		:=-iquote ${ROOT}
LIBS		:=

#	INTERNAL VARIABLES
OBJ		:=# .o files
DEP		:=# .d files
ASM		:=# .s files
DEPFLAGS	:=-MMD -MP# preprocessor generates .d files
ASMFLAGS	:=-S# source code commented assembly code 
.SUFFIXES:
```

The comments are very self-explanatory. I will only explain the last line:
```
.SUFFIXES:
```
It undefines any existing pattern rules. The default pattern rules will conflict with the pattern rules defined in `rules.mk` files.

## Constructing Makefiles from Modules

Take `demo/sort/Makefile` as an example:
```
ROOT    :/path/to/zmake/demo
d       :=${ROOT}/sort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/sort/rules.mk
-include ${ROOT}/sort/heapsort/rules.mk
-include ${ROOT}/sort/quicksort/rules.mk
-include ${DEP}

.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	rm -f *.exe ${OBJ} ${DEP} ${ASM}

cmp_sort.exe: ${d}/cmp_sort.o ${d}/heapsort/heapsort.o ${d}/quicksort/quicksort.o \
	${ROOT}/utils/utils.o ${ROOT}/utils/Table.o
```
First tell the Makefile the path of the source tree (`ROOT`) and the current subdirecotry (`d`). Then include `root.mk`. Then include `rules.mk` files in **any order** you want. Then include .d files (`-include ${DEP}`). Then define some basic phony targets (`all asm clean`). Then list the dependencies (in the form of `target: prerequisites`) for **each executables** to build.

**NOTE**:
* The Makefile only uses absolute paths and filenames.

* This method has removed the support for out-of-source build. Please contact me at qzmfranklin_at_gmail_dot_com for more information.

* `d` stores the absolute path of *this directory*. It is only defined for convenience.

* The default recipe for .exe files are defined in `rules.mk` files. You can still overwrite it in the Makefile because explicit recipes overwrite pattern rules.


# The zmake Script
The `zmake` script can generate `root.mk`, `rules.mk` files, and `Makefile`s.

**NOTE**: The out-of-source build function is removed from the the `zmake` script. The old `zmake` that supports out-of-source build is renamed to `zmake2`.

**WARNING/TODO**: This part is slightly outdated. Will update soon!

## List of Requirement
* POSIX OS.
* Only support C/C++ projects.
* A working python3 interpreter.

## How to get help from the command line?
Make sure that the `zmake` script is included in `$PATH`, type

	zmake -h
	
Then you should see:

```
usage: zmake [-h] [-f | -s | -d] [-r | -R] [-g] [-o TARGET] [dir]

Generating module files for constructing a single Makefile

positional arguments:
  dir              directory of source files (.)

optional arguments:
  -h, --help       show this help message and exit
  -f, --force      force overwriting existing files (False)
  -s, --skip       skip any existing file (False)
  -d, --delete     recursively delete all .mk files in [dir] (False)
  -r, --root-only  generate [dir]/root.mk (False)
  -R, --recursive  recursively generate rules.mk's (False)

Makefile:
  -g, --makefile   generate a Makefile (False)
  -o TARGET        output the Makefile to TARGET (Makefile)
```


## Generate root.mk
In a terminal, `cd` into the root directory of `zmake`. Type

	zmake demo -r

You will see the following message:

	generated demo/root.mk

The positional argument `demo` is the root directory of all the source files. The flag `-r/--root-only` tells `zmake` to only generate `root.mk` in the `demo/` directory.

If you do not want to type the word `demo` in the terminal, you can try

	cd demo
	zmake -r

If `demo/root.mk` already exists, you will be prompted an overwrite notice:

	demo/root.mk already exists, overwrite (y/n/q)?
	
You may try either of y=yes, n=no, q=quit and it is *not* case sensitive.

If you type "y", the existing `root.mk` will be overwritten. If you type "n", `zmake` will skip the generation of `root.mk` and go on to its next target, if any. If you type "q", `zmake` will quit immediately.

## Generate `rules.mk` recursively
**After** the `root.mk` is generated, we can go on to generate the `rules.mk` files by typing (still inthe root directory of `zmake`)

	zmake demo -R
	
You will see something similar to the following message:

	generated /Users/qzmfrank/codes/zmake/demo/heapsort/rules.mk
	generated /Users/qzmfrank/codes/zmake/demo/utils/rules.mk
	
with "/Users/qzmfrank/codes/zmake" being replaced by the root directory of zmake on your own computer.

The option `-R/--recursive` instructs the `zmake` script to 

1. Search for `root.mk` in `demo/`. If there is no `root.mk` in `demo/`, search for `root.mk` in `demo/..`, and go on like so, until it reaches the `/` directory or the depth of search exceeds 99. If `root.mk` is not found, `zmake` prints `root.mk not found` and then exits with code 1. If `root.mk` is first found in directory `dir`, go to step 2.

2. Recursively scan `dir`, create `rules.mk` for each subdirectory in `dir` that has at least one `.c` file or `.cpp` file.

If some `rules.mk` already exists, then you will be prompted the overwrite notice a lot (same as the overwrite notice in `root.mk`). Sometimes that may be a little bit annoying. Depending on your need, you may either use

	zmake demo -R --force
	
to force overwrite, or

	zmake demo -R --skip
	
to skip creating any existing files.


**NOTE**: The `-f/--force` and `-s/--skip` options apply to `-r/--root-only`, `-b/--branch-only`, `-R/--recursive`, and `-g/--makefile`.

## Generate only one rules.mk
Sometimes it might be desirable to generate the `rules.mk` for a single subdirectory. The `zmake` script can do this too.

In terminal, in the root directory of `zmake`, type

	rm demo/utils/rules.mk
	zmake -b demo/utils
	
You will see

	generated demo/utils/rules.mk
	
This time, the `-b/--branch-only` options instructs the `zmake` script to only generate one `rules.mk` file.

## Delete `.mk` files

The option `-d/--delete` instructs the `zmake` script to 

1. Search for `root.mk` in `demo/`. If there is no `root.mk` in `demo/`, search for `root.mk` in `demo/..`, and go on like so, until it reaches the `/` directory or the depth of search exceeds 99. If `root.mk` is not found, `zmake` prints `root.mk not found` and then exits with code 1. If `root.mk` is first found in directory `dir`, go to step 2.

2. Recursively scan `dir`, delete all `root.mk` and `rules.mk` files.

For example, if you type

	zmake -d demo

You will see
	
	deleted demo/root.mk
	deleted demo/heapsort/rules.mk
	deleted demo/utils/rules.mk


### Generate in-source build `Makefile`s
Manually putting up a `Makefile` from all the `.mk` is a labor intensive job, at which computers excel. So let the computer do it.

The `zmake` script can generate `Makefile`s that uses either in-source build mode and out-of-source build mode. The default is in-source build. We will go over generating `Makefile`s that uses out-of-source build mode later.

**After** `root.mk` and neccessary `rules.mk` are generated, in the root directory of `zmake`, type

	zmake -g demo -o demo/heapsort/Makefile

You will see

	generated demo/heapsort/Makefile


The `-g/--makefile` option instructs the `zmake` script to first find `root.mk` and then generate a `Makefile`. The `-o TARGET` option redirects the `Makefile` to `TARGET`. The default value of `TARGET` is `./Makefile`.

Open the `demo/heapsort/Makefile` with your favorite editor, add
```
cmp_sort.exe: ${d}/cmp_sort.o ${d}/heapsort/heapsort.o ${d}/quicksort/quicksort.o \
    ${ROOT}/utils/utils.o ${ROOT}/utils/Table.o
```
at the end of the Makefile. Save, back to terminal, type
```
cd demo/sort
make cmp_sort.exe
```

If everything goes right, you can run the `cmp_sort.exe` to compare different sorting algorithms. It may take a while to run.

**NOTE**: The `-f/--force` and `-s/--skip` options work with `-g/--makefile` too.
