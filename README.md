zmake
=====
A new method of writing Makefiles.

# A Brief History of Makefiles

The reader is assumed to be familiar with targets, prerequisites, recipes, dependencies, Unix shell commands, and compiler options.

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

## The Necessity of a Build System

To build the .exe files, first, each .c/.cpp file is compiled with `-c` option into .o file (object file). Then, a number of .o files are linked into .exe files.

During the first stage, or the compiling phase, each .o file only depends on one .c/.cpp file, but may include, i.e., depend on many .h files. During the second stage, or the linking phase, each .exe file can depend on many .o files.

During the compiling phase, we call the .o files the **targets**, and the .c/.cpp/.h files the **prerequisites**. During the linking phase, we call the .exe files the targets, and the .o files the prerequisites. A target can **depend** on one or more prerequisites, meaning that if at least one of the prerequisites is updated (newer than the target), the target needs to be updated.

The dependencies of a given source tree can be formally expressed as a **directed acyclic graph (DAG)**. Imaginably, we the project grows large, a **build system** that can automatically update targets by inspecting the DAG is desirable.

## The make Program and the Makefile

The standard tool for constructing a build system is the GNU make program. The make program reads the DAG from a file called the Makefile and use the DAG to update the targets that the user specified through the command line argument.


In the Makefile's syntax, dependencies between targets and prerequisites are expressed in the following way:

    target: prerequisite1 prerequisite2 ...
        recipe

The 'recipe' on the second line is 8-space tabbed (the make program has been very stubborn on insisting 8-space tab). It is the command that is to be invoked when the target is outdated. More specifically, if the make program detects that any of the prerequisites is modified after the last modified date of the target, the make program invokes the 'recipe' in an attempt to update the target.

Of course, if the DAG is complicated, the order in which different recipes are invoked can be vital. And parallel make, which attempts to invoke more than one recipes at the same time, while being highly desirable because of its efficiency, is hard to achieve without the make program.

## Brief Summary of Known Methods of Writing Makefiles

In principle, one can list dependencies by hand in a *monolithic* Makefile. In practice, nobody cares to do that because

> As the project grows larger, it would at some point become practically impossible to read, modify, and maintain the monolithic Makefile.

Pattern rules such as `.o .c` and `%.o : %.c` can save some work. But such pattern rules do not permit ***directory-specific*** compiling and linking options, which are the keys to achieving ***directory level modularity***.

The method of *recursive make* can easily achieve the directory level modularity. But, as Peter Miller pointed out in his famous [*Recursive make considered harmful*](http://aegis.sourceforge.net/auug97.pdf), recursive make breaks the completeness of the DAG. This has several consequences:

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


# A New Method of Writing Makefiles

## Introduction to Modules
The above discussion how to write Makefiles lead us to develop **a new method of writing a single Makefile that supports directory-specific pattern rules**.

Similar to Peter's and Emile's methods, my method has a `rules.mk` file in each subdirectory. Take a look at `demo/sort/quicksort/rules.mk` as an example:

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
The `zmake` script the process of generating a valid build system using the above-mentioned method.

**NOTE**: The out-of-source build function is removed from the the `zmake` script. The old `zmake` that supported out-of-source build was renamed to `zmake2`. It is now obsolete. Please do not use it.

## List of Requirement
* POSIX OS.
* Only support C/C++ projects (yet).
* A working python3 interpreter. Although you can make the script work with python2 by changing the print() statements.

## Get help/documentation

The directory `demo/` in this git repo has a lot of useful information.

Or, you can get help from the command line.

Make sure that the `zmake` script is included in `$PATH`, type

	zmake -h

Then you should see:

```
usage: zmake [-h] [-f | -s | -d] [-r | -R] [-g | --renew] [dir]

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
  -g, --makefile   generate a Makefile (False)
  --renew          renew ${ROOT} in all Makefiles (False)
```

## Generate root.mk
In a terminal, `cd` into the root directory of `zmake`. Type

    cd demo
    zmake -r

You will see the following message:

    generated ./root.mk

The flag `-r/--root-only` tells `zmake` to only generate `root.mk`.

If `./root.mk` already exists, you will be prompted an overwrite notice:

    demo/root.mk already exists, overwrite (y/n/q)?

You may try either of y=yes, n=no, q=quit and it is *not* case sensitive.

If you type 'y', the existing `root.mk` will be overwritten. If you type 'n, `zmake` will skip the generation of `root.mk` and go on to its next target, if any. If you type "q", `zmake` will quit immediately.

**NOTE**: Right now, typing anything but y/n/q will lead to abortion of program. But it does not harm. May take care of this issue later.

## Generate all the `rules.mk` files with one command
**After** the `root.mk` is generated, we can go on to generate the `rules.mk` files by typing (still inthe root directory of `zmake`)

    zmake -R -s

You will see something similar to the following message:

    generated /Users/qzmfrank/codes/zmake/demo/heapsort/rules.mk
    generated /Users/qzmfrank/codes/zmake/demo/utils/rules.mk
    ...(not listed)

with "/Users/qzmfrank/codes/zmake" being replaced by the root directory of zmake on your own computer.

The option `-R/--recursive` instructs the `zmake` script to

1. Search for `root.mk` in `demo/`. If there is no `root.mk` in `demo/`, search for `root.mk` in `demo/..`, and go on like so, until it reaches the `/` directory or the depth of search exceeds 99. If `root.mk` is not found, `zmake` prints `root.mk not found` and then exits with code 1. If `root.mk` is first found in directory `dir`, go to step 2.

2. Recursively scan `dir`, create `rules.mk` for each subdirectory in `dir` that has at least one `.c` file or `.cpp` file.

If some `rules.mk` files already exists, then you will be prompted the overwrite notice a lot (same as the overwrite notice in `root.mk`). Sometimes that may be a little bit annoying. Depending on your need, you may either use

    zmake demo -R --force

to force overwrite, or

    zmake demo -R --skip

to skip creating any existing files.

## Generate only one rules.mk
Sometimes it might be desirable to generate the `rules.mk` for a single subdirectory. The `zmake` script cannot do this yet. Explanation is available. In short, that is because the make program has undefined behavior when you reused a variable name to define pattern rules.

If you just want to create one `rules.mk` file, use the `-s/--skip` flag:

    zmake <dir> -R -s

## Delete `.mk` files

Deleting all the `.mk` files can be nasty if you have lots of directories. You can use

    zmake -d

to do this job.

### Generate `Makefile`s (in-source)
Manually putting up a `Makefile` from all the `.mk` is a labor intensive job. Let let the computer do it.

**After** `root.mk` and necessary `rules.mk` are generated, in `demo/`, type

    cd sort
    zmake -g

The `-g/--makefile` option instructs the `zmake` script to first find `root.mk` and then generate a `Makefile`.

Open the `demo/heapsort/Makefile` with your favorite editor, add
```
cmp_sort.exe: ${d}/cmp_sort.o ${d}/heapsort/heapsort.o ${d}/quicksort/quicksort.o \
    ${ROOT}/utils/utils.o ${ROOT}/utils/Table.o
```
at the end of the Makefile. Save, back to terminal (in `demo/sort/`), type

    make cmp_sort.exe
    ./cmp_sort.exe

**NOTE**: The Maverick (OSX 10.9) stdlib has mergsort declared in `/usr/include/stdlib.h`. Build will fail. But on Ubuntu 14.04 everything works well. Will fix this, later...

**NOTE**: Mac users may try (from the `demo/` directory)

    cd comp_geo/closest_pair
    make test_closest_pair.exe time_closest_pair.exe
    ./test_closest_pair.exe
    ./time_closest_pair.exe

The `test_closest_pair.exe` finds the closest pair of points among a lot of randomly genrated 2D points. The `time_closest_pair.exe` measures the speed of the implementation in terms of CPU cycles. The result confirms the scaling is nlgn, as expected.
