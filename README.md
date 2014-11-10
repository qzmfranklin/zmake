zmake
=====
A new method of writing Makefiles.

**NOTE**: We use `root/` to denote the directory where you cloned zmake.git on your own computer.

**NOTE**: This README.md is outdated. Will update as soon as the changes are stablized and tested.

**TODO**: So far, zmake is no more than a method and a script for writing Makefiles. The idea of using complete DAGs and having directory-specific pattern rules is restricted by the syntax and quirks and the time honored make program. A new make program that is built with easy specification of the DAG and directory-specific pattern rules is currently underway. As of 11/09/2014, only part of the DAG class is implemented, as in `demo/algorithm/graph/dag/`. Need a parser to parse the new Makefiles. A Nearly fully parallelized scheduler is already implemented in dag::schedule().

# A Brief History of Makefiles

The reader is assumed to be familiar with Unix/Linux shell commands, compiler options, and some GNU Makefile terminologies such as targets, prerequisites, dependencies, and recipes.


## Sample Project

`root/demo/` is a sample project:

> The source tree has grown significantly since the early days of this git repo. A partial display of the source as below should be enough for exposition purposes.

```
root/demo/
├── algorithm/sort/
│   ├── cmp_sort.cpp
│   ├── cmp_sort.exe
│   ├── heapsort/
│   │   ├── heapsort.cpp
│   │   ├── heapsort.h
│   │   ├── test_heapsort.cpp
│   │   └── test_heapsort.exe
│   └── quicksort/
│       ├── quicksort.cpp
│       ├── quicksort.h
│       ├── test_quicksort.cpp
│       └── test_quicksort.exe
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

There are two stages to building the .exe files. First, each .c/.cpp file is compiled with the `-c` option into .o file (object file). Second, object files are linked into .exe files.

During the first stage, or the compiling phase, each .o file only depends on only one .c/.cpp file, but may depend on many .h files. During the second stage, or the linking phase, each .exe file may depend on one or more .o files.

During the compiling phase, the .o files are the output, or the **targets**, and the .c/.cpp/.h files are the input, or the **prerequisites**. During the linking phase, the .exe files are the targets, and the .o files are the prerequisites. A target can **depend** on one or more prerequisites, meaning that if any of the prerequisites is updated - has a more recent last modified date than the target - the target needs to be updated too. The commands that are used to update targets are called **recipes** associated with the targets.

A project of compiling languages (C/C++/Fortran/Java) may have very complicated dependence relations. The resulted dependencies can be mathematically expressed in a **directed acyclic graph (DAG)**. The complexity of the DAG grows out of hand when the project grows. Tools that can manage the DAG, find outdated targets, and invoke associated recipes in a proper order are found wanting.

Such tools are build tools. Build tools, together with their accompanying scripts/programs/data, are called build systems.

A project of compiling languages such as C/C++/Fortran/Java may demand the following functionalities from its build system:
  1. The build system should go over the DAG once and only once. In another word, the build should be a one pass build.
  2. The build system should be fast and responsive. For example, it should not take more than a few seconds to figure out that nothing needs to be done.
  3. The build system should fully support specifying compiling flags at the directory level without having to provide redundant information. This is the key to achieving **directory level modularity**.


## The GNU make Program and the Makefile

The standard tool for constructing a build system is the GNU make program. The way it works is :
  1. The programmer writes the DAG and the recipes into a file named **Makefile**.
  2. When the programmer wishes to build the project, he invokes the make program.
  3. The make program reads in the DAG and the recipes from the Makefile.
  4. The make program traverses the DAG to find all outdated targets.
  5. The make program invokes associated recipes in a proper order.

The actions from step 2 through 5 can be collectively called a build, or a **make**. Parallel make can be enabled with the `make -jX` option (X is the number of threads).


## Brief Summary of Known Methods of Writing Makefiles


In the Makefile's syntax, dependencies between targets and prerequisites can be expressed in two ways.

* Explicit listing:
```
target: prerequisite1 prerequisite2 ...
        recipe
```
* Pattern rules: such as the following
```
%.o: %.c
        gcc -o $@ $^ ${CFLAGS}
```

Exhaustive listing of all dependencies would result in gigantic Makefiles that are impossible to maintain. Pattern rules, in their primitive format, do not support directory-specific compiling options.

The method of **recursive make** can achieve the directory level modularity. But, as Peter Miller pointed out in his famous [*Recursive make considered harmful*](http://aegis.sourceforge.net/auug97.pdf), recursive make breaks the completeness of the DAG, which has several consequences:

  1. The incompleteness of the DAG means that the make program will usually build too little.
  2. Patching the DAG would usually result in over-complete Makefiles that build too much.
  3. Recursive make is a multi-pass build method in nature. It can take an excessive amount of time to figure out "nothing needs to be done".
  4. Parallel make using the `make -j` becomes very tricky, if at all possible.

Peter Miller concluded that
> the right way of writing Makefiles was to write a single Makefile the could build the entire project.

Peter Miller proposed an *alternative single Makefile solution* (section 7 'Big Picture' in his paper). His solution had a file called `modules.mk` in each subdirectory to store directory specific information, and had a Makefile in the root directory include all the `modules.mk` files. However, his solution did not fully support directory level modularity because it passed compiling options using a global variable.

Emile van Bergen proposed a [new solution](http://evbergen.home.xs4all.nl/nonrecursive-make.html).

Similar to Peter's approach, Emile's solution was to write a `Rules.mk` in each directory and have the Makefile in the root directory include all the `Rules.mk` *in the right order*.

Emile's solution used stacks and stack pointers to keep track of paths. As the make program enters a directory `dir/subdir/` from `dir/`, the parent directory `dir/` is pushed into a stack, and the stack pointer is shifted to point to the child directory `dir/subdir/`. Then the make program `include` the `Rules.mk` pointed to by the stack pointer (`dir/subdir/`) to read in the DAG and recipes. Last, as the make program leaves `dir/subdir/`, the `dir/subidr/` is popped from the stack, and the stack pointer points to `dir/` again.

The advantages of Emile's solution are:

  1. `Rules.mk`s are oblivious of each other.
  2. It allows programmers to specify recipes for each *target* (one level below directory).

The disadvantages of Emile's solutions are:

  1. The correctness of the `Makefile` depends on the order in which the `Rules.mk` files are included because of the use of stack.
  2. In most situations, "recipe for each target" it is an overkill. Programmers would still have to list recipes (compiling options) for each target.

# A New Method of Writing Makefiles

## Introduction to Modules
The above discussion on how to write Makefiles lead us to develop **a new method of writing a single Makefile that supports directory-specific pattern rules and can build in one pass**.

Similar to Peter's and Emile's methods, this new method has a `rules.mk` file in each subdirectory. Take a look at `demo/sort/quicksort/rules.mk` as an example:

```
#  THIS DIRECTORY
DIR00006:=${ROOT}/algorithm/sort/quicksort
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

* `ROOT` stores the *absolute path* of the root of the source tree. `ROOT` is defined in Makefiles as a global variable, e.g., in this example, `ROOT:=~/git/zmake/demo`.

* `DIR00006` is a unique identifier of the `quicksort/` directory. It stores the *absolute path* of this directory (in this case, `${ROOT}/sort/quicksort`). **NOTE**: As of 09/06/2014, the `DIRXXXXX` is replaced by a very long hash string. `DIRXXXXX` is only used here for clarity.

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
${ROOT}/algorithm/sort/quicksort/*.o: ${ROOT}/algorithm/sort/quicksort/*.cpp
```
But it does **not** apply to targets in other directories. For example, it does not apply to
```
${ROOT}/algorithm/sort/cmp_sort.o: ${ROOT}/algorithm/sort/cmp_sort.cpp
```
Because `${${DIXXXXX}CXXFLAGS}` and `${${DIRXXXXX}INCS}` are unique to each subdirectory, the pattern rules defined in `rules.mk` only apply to their own subdirectory.

The `demo/root.mk` mentioned before is (with minor modifications):
```
#	PROJECT-WIDE COMMON COMPILING FLAGS
CC        :=g++
CFLAGS    :=-O3 -Wall -Drestrict=__restrict__

CXX       :=${CC}
CXXFLAGS  :=${CFLAGS}

#       PROJECT-WIDE DEFAULT LINKING LIBRARIES AND INCLUDE DIRECTORIES
INCS      :=-iquote ${ROOT}
LIBS      :=

#	INTERNAL VARIABLES
OBJ       :=# .o files
DEP       :=# .d files
ASM       :=# .s files
DEPFLAGS  :=-MMD -MP# preprocessor generates .d files
ASMFLAGS  :=-S# source code commented assembly code
.SUFFIXES:
```

The comments are very self-explanatory. I will only explain the last line:
```
.SUFFIXES:
```
It undefines any existing pattern rules (including those default pattern rules of the GNU make program). The default pattern rules will conflict with the pattern rules defined in `rules.mk` files in case of in-source build.

## Constructing Makefiles from Modules

Take `demo/algorithm/sort/Makefile` as an example:
```
ROOT    :/path/to/zmake/demo
d       :=${ROOT}/algorithm/sort# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/algorithm/sort/rules.mk
-include ${ROOT}/algorithm/sort/heapsort/rules.mk
-include ${ROOT}/algorithm/sort/quicksort/rules.mk
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

* The zmake script has removed the support for out-of-source build. If the demand for an out-of-source build version rises, I may make one.

* `d` stores the absolute path of *this directory*. It is only defined for convenience.

* The pattern rules for .exe files are stored in `rules.mk` files. If you want, however, you can overwrite pattern rules with explicit recipes.


# The zmake Script
The `zmake` script streamlines the process of generating a valid build system using the above-mentioned method.

## List of Requirement
* POSIX OS.
* Only support C/C++ projects (yet).
* A working python3 interpreter.
* The following python3 modules:
```
import hashlib
import shutil
import os
import glob
import fnmatch
import sys
import argparse
```

## Get help/documentation

The `root/demo/` has a lot of useful examples.

Or, you can get help from the command line. Make sure that the `zmake` script is included in `$PATH`, type

	zmake -h

Then you should see:

```
usage: zmake [-h] [-f | -s] [-d | -b | -r | -R] [-g | --renew] [dir]

Generating module files for constructing a single Makefile

positional arguments:
  dir                directory of source files (.)

optional arguments:
  -h, --help         show this help message and exit
  -f, --force        force overwriting existing files (False)
  -s, --skip         skip any existing file (False)
  -d, --delete       recursively delete all .mk files in [dir] (False)
  -b, --branch-only  generate rules.mk for a single directory (False)
  -r, --root-only    generate [dir]/root.mk (False)
  -R, --recursive    recursively generate rules.mk's (False)
  -g, --makefile     generate a Makefile (False)
  --renew            renew ${ROOT} in all Makefiles (False)
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

    generated /Users/qzmfrank/codes/zmake/demo/algorithm/sort/heapsort/rules.mk
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
Sometimes it might be desirable to generate the `rules.mk` for a single subdirectory. The `zmake` script can do this by

    zmake --branch-only demo/algorithm/sort

`--force/-f` and `-skip/-s` works with `--branch-only/-b`.
## Delete `.mk` files

Deleting all the `.mk` files can be nasty if you have lots of directories. You can use

    zmake -d

to do this job. Note though that `zmake -d` does NOT delete any Makefiles.

## Generate `Makefile`s (in-source)
Manually putting up a `Makefile` from all the `.mk` is a labor intensive job. Let let the computer do it.

**After** `root.mk` and necessary `rules.mk` are generated, in `demo/`, type

    cd algorithm/sort
    zmake -g

The `-g/--makefile` option instructs the `zmake` script to first find `root.mk` and then generate a `Makefile`.

Open the `demo/algorithm/sort/heapsort/Makefile` with your favorite editor, add
```
cmp_sort.exe: ${d}/cmp_sort.o ${d}/heapsort/heapsort.o ${d}/quicksort/quicksort.o \
    ${ROOT}/utils/utils.o ${ROOT}/utils/Table.o
```
at the end of the Makefile. Save, back to terminal (in `demo/sort/`), type

    make cmp_sort.exe
    ./cmp_sort.exe

## Renew Existing Makefiles for a New Path

The global variable `ROOT:=path/to/project` is hardcoded into Makefiles. When you copy your project from `dir1/` to `dir2/`, the `ROOT` does not change. You may use

```
zmake --renew
```

to update the Makefile in the current directory `./`. Or you can use

```
zmake --renew path/to/another/dir
```
to update the Makefile in any directory.

If you want to update all Makefiles searchable by zmake, add `-R` to --renew:
```
zmake --renew -R
```

The way the `--renew` flag works is

  1. It instructs the zmake script to find the root.mk. Denote the directory containing root.mk as `root/`.
  2. Recursively scan all subdirectories in `root/` for Makefiles. Change the `ROOT:=old/path/` to `ROOT:=new/path/`.
