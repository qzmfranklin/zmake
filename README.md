zmake
====

# LICENSE
This public git repository is licensed under GPL3, plus the following non-commercial condition:

- Use of any part of the code or methodology, direct or indirect, explicit or
  implicit, for building or testing any part of a commercial product is strictly
  prohibited. Adapting existing system to using the zmake method, or adapting
  the zmake scripts to use for building or testing is considered violation of
  this clause.

If you need a commercial license, please contact the author for endorsement.


# Overview

## What is zmake?

> MUST READ THIS PART BEFORE MOVING FORWARD

- The **zmake method** is a novel method of writing Makefiles for C/C++ projects.

- The zmake method is **modular**, **non-recursive** and builds targets **incrementally** in only **one pass**.

- The **zmake program** is a python3 program for generating Makefile modules that implement the zmake method.

- The **zmake repository** is a collection of the zmake program of many demo programs that demonstrate the use of the zmake method.

- The meaning of the word **zmake**, when used alone, should be clear from the context.


## What does the zmake method do as a build system?


- Automatic construction and tracking of dependencies.

- Guaranteed one-pass and incremental build.

- You can `make` from any directory, not only from the root directory of your project.

- Easy integration with the GoogleTest framework. Use `zmake --install-gtest` to download and install the gtest library, write your own tests, then `make test` will build your tests and run them.

- Originally, the author developed zmake for his personal projects at the University of Michigan. It suits the use of scientific programming very well.


## What zmake is NOT?

- It is *not* a full blown build system that attempts to meet every possible demand. Instead, it is lightweight.

- It is *not* a substitue for your own build system if your project is sitting on XCode or Android Studio. Again, it is lightweight.

- It only supports in-source build at the moment. If you need out-of-source build, please let me know.


# What is the zmake method?

> The reader is assumed to be familiar with Unix/Linux shell commands, common C/C++ compiler flags, and some GNU Makefile terminologies such as targets, prerequisites, dependencies, and recipes.

To fully explain the zmake method, we need to start by looking at the concepts of compilation, linking, dependency, Makefiles and the traditional ways of writing Makefiles.

## Sample Project

`root/demo/` is a sample project:

> The source tree has grown significantly since the early days of this git repo. A partial display of the source as below should be enough for exposition purposes.

```
demo/
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

## Why do we need a build system?

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

> The rules.mk file and some parts of the root.mk file you see by now is
  different from the one listed below. The newer versions added more features
  but the core methodology remained the same.


```
#  THIS DIRECTORY
TMP:=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
$(TMP)DIR:=$(TMP)

#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
$($(TMP)DIR)C  :=$(wildcard $(TMP)/*.c)
$($(TMP)DIR)CC :=$(wildcard $(TMP)/*.cc)
$($(TMP)DIR)CPP:=$(wildcard $(TMP)/*.cpp)
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
$($(TMP)DIR)CFLAGS:=$(CFLAGS)
$($(TMP)DIR)CXXFLAGS:=$(CXXFLAGS)
$($(TMP)DIR)INCS:=$(INCS)
$($(TMP)DIR)LIBS:=$(LIBS)

DEP:=$(DEP) $($(TMP)C:%.c=%.d) $($(TMP)CC:%.cc=%.d) $($(TMP)CPP:%.cpp=%.d)
OBJ:=$(OBJ) $($(TMP)C:%.c=%.o) $($(TMP)CC:%.cc=%.o) $($(TMP)CPP:%.cpp=%.o)
ASM:=$(ASM) $($(TMP)C:%.c=%.s) $($(TMP)CC:%.cc=%.s) $($(TMP)CPP:%.cpp=%.s)

$($(TMP)DIR)/%.o: $($(TMP)DIR)/%.c
	$(QUIET)$(CC) -o $@ -c $< $(DEPFLAGS) $($($(TMP)DIR)CFLAGS) $($($(TMP)DIR)INCS)
	$(QUIET)echo "make $(GREEN)$@ $(NONE)"
$($(TMP)DIR)/%.s: $($(TMP)DIR)/%.c
	$(QUIET)$(CC) -o $@ $< $(ASMFLAGS) $($($(TMP)DIR)CFLAGS) $($($(TMP)DIR)INCS)
	$(QUIET)echo "make $(CYAN)$@ $(NONE)"

$($(TMP)DIR)/%.o: $($(TMP)DIR)/%.cc
	$(QUIET)echo "make $(GREEN)$@ $(NONE)"
	$(QUIET)$(CXX) -o $@ -c $< $(DEPFLAGS) ${$($(TMP)DIR)CXXFLAGS} ${$($(TMP)DIR)INCS}
$($(TMP)DIR)/%.s: $($(TMP)DIR)/%.cc
	$(QUIET)echo "make $(CYAN)$@ $(NONE)"
	$(QUIET)$(CXX) -o $@ $< $(ASMFLAGS) ${$($(TMP)DIR)CXXFLAGS} ${$($(TMP)DIR)INCS}

$($(TMP)DIR)/%.o: $($(TMP)DIR)/%.cpp
	$(QUIET)echo "make $(GREEN)$@ $(NONE)"
	$(QUIET)$(CXX) -o $@ -c $< $(DEPFLAGS) $($($(TMP)DIR)CXXFLAGS) $($($(TMP)DIR)INCS)
$($(TMP)DIR)/%.s: $($(TMP)DIR)/%.cpp
	$(QUIET)echo "make $(CYAN)$@ $(NONE)"
	$(QUIET)$(CXX) -o $@ $< $(ASMFLAGS) $($($(TMP)DIR)CXXFLAGS) $($($(TMP)DIR)INCS)

# Linking pattern rule for this directory
%.exe: $($(TMP)DIR)/%.o
	$(QUIET)echo "make $(MAGENTA)$@ $(NONE)"
	$(QUIET)$(CXX) -o $@ $^ $($($(TMP)DIR)LIBS)

# Recursive inclusion
-include $(wildcard $(TMP)/*/$(notdir $(lastword $(MAKEFILE_LIST))))
```


* `MAKEFILE_LIST` is a variable maintained by the make program. It stores all
  files that are included, including the original Makefile. It is guaranteed to
  put the last included file, which is this rule.mk file in discussion at the
  end of the list. Therefore $(lastword $(MAKEFILE_LIST)) is just the file name
  of this rules.mk file.

* `TMP` takes the real path of the directory name of $(lastword
  $(MAKEFILE_LIST)). It is guaranteed to be unique across the entire project as
  long as there is no remote files symlinked to existing files.

* `$(TMP)DIR` is the namespace, so to speak, of this ruls.mk file. For example,
  when `$(TMP)` is evaluated to `/path/to/this/rulesmk`, `$($(TMP)DIR)` is
  evaluated to `/path/to/this/rulesmkDIR`, which is guaranteed to be unique as
  long as `$(TMP)` is the real path, i.e., starting from `/` and with symlink
  resolved.

* `$($(TMP)DIR)CFLAGS)` is the compilation flags that are used for compiling
  C files in this directory. Similarly `$($(TMP)DIR)CXXFLAGS` is the compilation
  flags for C++ files in this directory. Note that because `$(TMP)DIR` has
  unique value, the compiling flags of this directory will not tamper with those
  of other directories.

* The pattern rules are listed for .c, .cc, and .cpp files. Without losing
  generality, we use the pattern rules for .cpp files for exposition. First
  let's look at the pattern matching: ``` $($(TMP)DIR)/%.o: $($(TMP)DIR)/%.cpp
  ``` (a). Pattern rule (a) differs from ``` %.o: %.cpp ``` (b) in an important
  way. (a) only matches .cpp files with .o files in a certain directory but (b)
  matches .cpp files with .o files in all directories. This is how zmake
  achieved directory level modularity.

* The dependency of .o files on the .h files are automatically tracked by the .d
  files generated by the `-MMD -MP` preprocessing flags. The Makefile will try
  to include .d files it needs to compile .o files for by the `-include`
  directive. It is the makefile's perk that if it fails to include a target, it
  treats it as a target and try to make it. The .d files are thus asked to be
  generated. It might seem that we need to add a `%.d: %.cpp` pattern rule for
  the .d files. But surprisingly (I do not understand why either) enough,
  everythin works just fine without the `%.d: %.cpp` pattern rules. If you can
  help me understand why, I would like to thank you with a cup of starbuck
  coffee.

* The pattern rule for linking is different from the pattern rules for the .o
  files. The main reason is to allow the Makefiles constructed from these
  rules.mk files, as we shall see later, to easily specify the linking
  dependencies, thus minimizing developer work.

* The last line simply tries to include the rules.mk files in the subdirectories
  of this directory. With this recursive inclusion, you only need to include
  very few rules.mk files in Makefiles constructed from rules.mk files.

* `QUIET` is a global variable defined in the root.mk file. It should be either
  undefined or defined by be a single @ character. In the later case, the
  recipes in the rules.mk file, when invoked and executed, do not get printed to
  stdout.

* The reason for using `QUIET` is completely aethetic. Because zmake uses full
  path for directories and files, the recipes, when expanded, are not readible.
  In addition to hiding the full command, we need to print out something
  indicating to the developer what the make program is doing. This is done by
  the echo commands in the recipes in the rules.mk files.

* `CXX` is the C++ compiler. It is defined in the `demo/root.mk` file.

* `DEPFlAGS` is defined in `demo/root.mk`: `DEPFLAGS:=-MMD -MP`. It instructs
  the compiler to generate .d files.

* `ASMFLAGS` is defined in `demo/root.mk`: `ASMFLAGS:=-S`. It instructs the
  compiler to generate assembly listing files.

* Source files are picked up by the Makefile wildcard command, so that when you
  add files, remove files, or change file names the build system picks up
  changes automatically for you.

The `demo/root.mk` mentioned before is (with minor modifications):
```
#	PROJECT-WIDE COMMON COMPILING FLAGS
CC		:=clang
CFLAGS 		:=-O3 -Wshadow -Wno-unused-result -Drestrict=__restrict__ \
	-DNDEBUG -fpermissive -pthread

CXX		:=clang++
CXXFLAGS	:=$(CFLAGS) -std=c++11

#       PROJECT-WIDE DEFAULT LINKING LIBRARIES AND INCLUDE DIRECTORIES
INCS		:=-iquote $(ROOT) \
			-isystem $(ROOT)/gtest/include \
			-isystem $(ROOT)/gtest
LIBS		:=

QUIET		:=@

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

It undefines any existing pattern rules (including those default pattern rules
of the GNU make program). The default pattern rules will conflict with the
pattern rules defined in `rules.mk` files in case of in-source build.


## Constructing Makefiles from Modules

Once we have the root.mk file and the ruls.mk file(s), the next step is to
construct Makefile.

Take `demo/algorithm/sort/Makefile` as an example:

```
ROOT    :=$(realpath ../..)
D       :=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))# this directory
-include $(ROOT)/root.mk
-include $(ROOT)/utils/rules.mk
-include $(ROOT)/gtest/rules.mk
-include $(D)/rules.mk
# Inlcude more rules.mk here if you this directory depends on them.
-include $(DEP)

.PHONY: all asm clean test
all: $(OBJ)
asm: $(ASM)
clean:
	$(QUIET)rm -f *.exe $(OBJ) $(DEP) $(ASM)

cmp_sort.exe: $(D)/cmp_sort.o \
	$(D)/heapsort/heapsort.o \
	$(D)/quicksort/quicksort.o \
	$(ROOT)/utils/utils.o \
	$(ROOT)/utils/Table.o
```

* `ROOT` is the the real path of the directory that contains the root.mk file
  directly.

* `-include`d rules.mk files will recursively include ruls.mk files in their own
  subdirectories. Therefore we do not need to include `$(D)/quicksort/rules.mk`
  or `$(D)/heapsort/rules.mk`.

* `D` stores the absolute path of *this directory*. It is only defined for
  convenience.

* The pattern rules for .exe files are stored in `rules.mk` files. If you want,
  however, you can overwrite pattern rules with explicit recipes.


# The zmake Script
The `zmake` script streamlines the process of generating a valid build system
using the above-mentioned method.

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
usage: zmake [-h] [-C DIRECTORY] [-R] [-f | -s] (-b | -D | -g | -m | -r)

Version: 0.9. Generating module files for constructing a single Makefile

optional arguments:
  -h, --help            show this help message and exit
  -C DIRECTORY, --directory DIRECTORY
                        the directory to work from (default: .)
  -R, --recursive       recursively, works with -r, -b, -m (default: False)
  -f, --force           force overwriting existing files (default: False)
  -s, --skip            skip any existing file (default: False)
  -b, --branch          generate rules.mk for a single directory (default:
                        False)
  -D, --delete-recursively
                        recursively delete all .mk files in, implies -R and -f
                        (default: False)
  -g, --install-gtest   install gtest with customized zmake-like Makefile and
                        modules (default: False)
  -m, --makefile        generate a Makefile (default: False)
  -r, --root            generate [DIRECTORY]/root.mk (default: False)
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

```
make cmp_sort.exe
./cmp_sort.ex
```

## Out-of-source build
The zmake project can work with out-of-source build. Actually out-of-source
build was originally implemented but later removed because a) the zmake project is
moving forward very fast, and b) in-source build worked for me just fine.

If you need out-of-source build with zmake, it is definitely possible. Please
just leave a comment to let me know.
