zmake
=====
A new method of writing Makefiles.

## What's in a Makefile?
### The GNU make program and Makefiles
The `make` program takes a file, typically called `Makefile`, from the command line. The Makefile provides information on dependencies in the form of `target: prerequisite(s)`. There is usually also a `recipe` associated with each `target`. If the `target` is older than any of its `prerequisite`s, the `make` program will invoke the `recipe` associated with the `target`.

The dependencies in a project will form a *directed acyclic graph* (DAG). The `make` program relies on the DAG to function properly.

Makefiles are old, vast, and complicated. Writing good `Makefile`s is not easy.

To illustrate various approaches, let's first introduce a sample project.


#### Sample project

Suppose we have a C++ project with the following source tree:

```
.
├── heapsort/
│   ├── heapsort.cpp
│   ├── heapsort.h
│   └── test_heapsort.cpp
└── utils/
    ├── test_utils.cpp
    ├── utils.cpp
    └── utils.h
```
Suppose there are two executables to build: `test_heapsort.exe` and `test_utils.exe`. And we want to use `-O3` and `-g` as the compiling flags for `heapsort` and `utils`, respectively. We also want to link `test_heapsort.o` with `-lm` but link `test_utils.o` with no extra libraries. 

This is a small project. But it is complicated enough to demonstrate our points.


### The classic monolithic Makefile approach

Build the sample project using a monolithic Makefile (`demo0/Makefile`):

```
CXX:=icpc


CFLAGS1:=-O3
CFLAGS2:=-g
INCS:=-iquote .

test_heapsort.exe: test_heapsort.o heapsort.o utils.o
	${CXX} -o $@ $^ -lm

test_utils.exe: test_utils.o utils.o
	${CXX} -o $@ $^

test_heapsort.o: heapsort/test_heapsort.cpp \
			heapsort/heapsort.h utils/utils.h 
	${CXX} -o $@ -c $< ${CFLAGS1} ${INCS}

heapsort.o: heapsort/heapsort.cpp heapsort/heapsort.h
	${CXX} -o $@ -c $< ${CFLAGS1} ${INCS} 
	
test_utils.o: utils/test_utils.cpp utils/utils.h
	${CXX} -o $@ -c $< ${CFLAGS2} ${INCS}

utils.o: utils/utils.cpp utils/utils.h
	${CXX} -o $@ -c $< ${CFLAGS2} ${INCS}

.PHONY: all clean cleanxx

all: 

clean:
	rm -f *.o
cleanxx: clean
	rm -f *.exe
```

The method of writing a monolithic Makefile for a large project has a major disadavantage:

* Programmers have to manually list all the dependencies, including dependencies on `.h` files. As the project grows larger and larger, it would at some point become practically impossible to read, modify, and maintain.

In theory, pattern rules such as `.o .c` and `%.o : %.c` can be used to reduce the amount of manual work. But in practice, such pattern rules do not permit ***directory-specific*** recipes, which renders them almost useless for large projects that have different compiling and building rules for different directories.

To counter these issues, especially as an effort to achieve the modularity at the *directory level*, people found a new way of writing `Makefiles` - using *recursive `make`*.

### Recursive make considered harmful

Recursive make has good modularity. But, as Peter Miller pointed out in his famous [*Recursive make considered harmful*](http://aegis.sourceforge.net/auug97.pdf), recursive make breaks the completeness of the DAG. This has several consequencies:

* The incompleteness of the DAG means that the make program will usually build too little. 
* Manually "patching" the DAG would usually result in an over-complete Makefile that builds too much.
* Parallel make using the `-j` flag becomes very tricky, if at all possible.
* Makefiles written using recursive make are not easy to maintain, either.

Understandably, these problems are hard to resolve within the scope of the recursive make method. So, Peter Miller came to the conclusion that recursive make was harmful and the right way of writing Makefiles was to write a single Makefile the could build the entire project. 

Peter Miller proposed an alternative single Makefile solution (section 7 Big Picture in his paper). His solution had a file called `modules.mk` in each subdirectory that actually contains source files, and had the `Makefile` in the root directory include all the `modules.mk` files. His solution was still using a global `${CFLAGS}` variable to pass the compiling options, which was not fit for achieving the desired *directory-specific pattern rules*.

Emile van Bergen, was aware of these, and proposed a new solution.

### The single `Makefile` solution due to Emile van Bergen
The implementation due to [Emile](http://evbergen.home.xs4all.nl/nonrecursive-make.html) is a single `Makefile` approach that can achieve the modularity at the directory level. 

Its basic idea is to write `Rules.mk` files for each subdirectory that actually contains source files, and have the `Makefile` in the root directory inlcude all the `Rules.mk` in the right order.

Emile's solution uses stacks and stack pointers. As the `make` program enters a `Rules.mk` (assuming `Rules.mk` is in a subdirectory called `subdir`), the parent directory of `subdir` is pushed into a stack and the stack pointer shifts to `subdir`. As the `make` leaves `subdir`, the parent directory is poped from the stack and the stack pointer shifts back to the parent directory.

The advantages of Emile's solution are:

* Each `Rules.mk` is oblivious of any other `Rules.mk`'s. This is a significant progress towards better modularity.
* In principle, Emile's solution allows programmers to specify recipes for each *target*.

The disadvantages of Emile's solutions are:

* The use of stacks introduces an extra layer of indirection. The correctness of the `Makefile` depends on the order in which the `Rules.mk` files are included.
* It does not support *directory-specific pattern rules*. Although it support *target-specific recipes*, sometimes it is just an overkill and much labor is still needed to list all the recipes.

Given these, we may wish to have a yet better single Makefile solution.

## A new single Makefile solution
In this part, we will introduce a new method for writing Makefiles.

### Sample project again
Let's demonstrate the method with a sample project. The codes are in `demo1/`. The sample project is explained in a previous section. For convenience the source tree is listed below:

```
demo1/
├── heapsort/
│   ├── heapsort.cpp
│   ├── heapsort.h
│   └── test_heapsort.cpp
└── utils/
    ├── test_utils.cpp
    ├── utils.cpp
    └── utils.h
```
### Overall structure of the Makefiles
Our method has `rules.mk` files in every subdirectory has actually contains source files and a `root.mk` in the root directory. Makefiles are constructed using these .mk files. All the files related to constructing the Makefile for `demo1/` are listed below:

```
demo1/
├── Makefile
├── heapsort/
│   └─── rules.mk
├── root.mk
└── utils/
    └─── rules.mk
```

In the following sections, we will go over Makefile, root.mk, and rules.mk one at a time.

### Makefile
The `demo1/Makefile`:

```
QUIET  :=@# comment this line to print more information 
ROOT    :=/media/MyData/codes/zmake/demo
d       :=${ROOT}/utils# this directory
-include ${ROOT}/root.mk
-include ${ROOT}/utils/rules.mk
-include ${ROOT}/heapsort/rules.mk
-include ${DEP}
################################################################################
.PHONY: all asm clean
all: ${OBJ}
asm: ${ASM}
clean:
	${QUIET}rm -f *.exe ${OBJ} ${DEP} ${ASM}
################################################################################
#	LIST YOUR LINKING RULES HERE
test_heapsort.exe: ${ROOT}/heapsort/heapsort.o \
	${ROOT}/heapsort/test_heapsort.o \
	${ROOT}/utils/utils.o
test_utils.exe: ${ROOT}/utils/test_utils.o \
	${ROOT}/utils/utils.o
```

* `QUIET` is a global flag. If `QUIET` is set to `@`, few messages are printed when using this Makefile.
* `ROOT` is the *absolute* path of the root directory of the source files.
* The root.mk file and the several rules.mk files are explained the the next sections. In short, root.mk stores the project-wide compiling options, include directories, and libraries to link against. The rules.mk files store pattern rules that are specific to each directory. The root.mk should always be included at the beginning of the Makefile. The rules.mk files should be included after the root.mk.
* `DEP` is all the dependency files, i.e., `.d` files. The minus sign before `include` suppresses unwanted warning messages.
* Several phony targets, i.e., all, asm, and clean are defined to make life easier. `OBJ` is all the `.o` files. `DEP` is all the `.d` files. `AMS` is all the `.s` files, i.e., assembly listing files.
* In the "LIST YOUR LINKING RULES HERE" section, list all the dependencies of the executables we want to build, and the `.o` files they depend on. Where are the recipes? There are pattern rules to do the build. The pattern rules are in the `rule.mk` files. We will expalin this in the next section.

**NOTE**: `ROOT` and `QUIET` are defined in the Makefile. `OBJ`, `DEP`, and `ASM` are defined in root.mk. Pattern rules, compiling options, and linking options that are specific to each subdirectory are defined in rules.mk files.


### root.mk

The `root.mk` contains the project-wide/ top-level information:

```
################################################################################
#	PROJECT-WIDE COMMON COMPILING FLAGS 
CC		:=icc
CFLAGS 	:=-O3							\
		-Wall							\
		-std=c99						\
		-Wno-deprecated

CXX		:=icpc
CXXFLAGS:=-O3							\
		-Wall							\
		-Wno-deprecated

#       PROJECT-WIDE DEFAULT LINKING LIBRARIES AND INCLUDE DIRECTORIES
INCS		:=-iquote ${ROOT}
LIBS		:=
################################################################################
#		INTERNAL VARIABLES
OBJ		:=# .o files
DEP		:=# .d files
ASM		:=# .s files
DEPFLAGS:=-MMD -MP# preprocessor generates .d files
ASMFLAGS:=-S -fsource-asm# source code commented assembly code 
.SUFFIXES:
################################################################################
# 	OUT-OF-SOURCE PATTERN RULES
ifeq (OUT_OF_SOURCE,TRUE)
${BUILD}/%.exe: ${BUILD}/%.o
	${QUIET}${CXX} -o $@ $^ ${LIBS} 
endif

```

* `CC` and `CXX` are the C and C++ compilers, respectively. 
* `CFLAGS` and `CXXFLAGS` are the project-wide C and C++ compiling options. We emphasize that the C and C++ compiling options can be fully customized for each subdirectory. `CFLAGS` and `CXXFLAGS` are only the default settings.
* `INCS` tells the include directories when compiling. `ROOT` is explained in the previous section about the Makefile. 
* The `-iquote dir` option is a preprocessor options that adds the directory `dir` to the search path for `include "..."` in `.c` and `.cpp` files. `-iquote ${ROOT}` simplifies the `include` directives. For example, `heapsort/test_heapsort.cpp` needs to include `utils/utils.h`. With `-iquote ${ROOT}`, a simple `include "utils/utils.h"` will do. This also makes the codes much less vunerable to changes in the structures of the source directories.
* `OBJ`, `DEP`, and `ASM` are explained in the comments.
* `DEPFLAGS`. The `-MMD -MP` options directs the compiler the generate `.d` files automatically. So that we no longer need to rely on the mysterious `%.d: %.c` rules.
* `ASMFLAGS`. The `-S` option directs the compiler the generate assembly listing files of the corresponding `.c`/`.cpp` file. The `-fsource-asm` option adds original C/C++ codes to the assembly listing files as comments. How to generate assembly listing file for `utils/utils.cpp`? Just type `make utils/utils.s`. We will explain how this works in the rule.mk section.
* `.SUFFIXES:` undefines any existing pattern rules. This is crucial for the directory-specific pattern rules to function correctly. We will explain this later in the rules.mk section.
* The "OUT-OF-SOURCE PATTERN RULES" section is not processed if `OUT_OF_SOURCE` is not `TRUE` (case sensitive). We will explain this part together with `OUT_OF_SOURCE` and `BUILD` when we explain out-of-source build.

### rules.mk

Let's look at the `utils/rules.mk`:


```
################################################################################
#  THIS DIRECTORY
DIR:=${ROOT}/utils
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR}C:=
${DIR}CPP:=test_utils.cpp utils.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR}CFLAGS:=${CFLAGS}
${DIR}CXXFLAGS:=${CXXFLAGS}
${DIR}INCS:=${INCS}
################################################################################ 
#		INTERNAL VARIABLES AND PATTERN RULES
ifeq (${OUT_OF_SOURCE},TRUE)
# out-of-source build
(omitted here...)
# in-place build
# Add local targets to global variables
DEP:=${DEP} ${${DIR}CPP:%.cpp=${DIR}/%.d} ${${DIR}C:%.c=${DIR}/%.d} 
OBJ:=${OBJ} ${${DIR}CPP:%.cpp=${DIR}/%.o} ${${DIR}C:%.c=${DIR}/%.o} 
ASM:=${ASM} ${${DIR}CPP:%.cpp=${DIR}/%.s} ${${DIR}C:%.c=${DIR}/%.s} 

# C sources
${DIR}/%.o: ${DIR}/%.c
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}
${DIR}/%.s: ${DIR}/%.c
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}

# C++ sources
${DIR}/%.o: ${DIR}/%.cpp
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS}
${DIR}/%.s: ${DIR}/%.cpp
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR}/%.o
	${QUIET}${CXX} -o $@ $^ ${LIBS}
endif 
```

* `DIR` stores the full path of the subdirectory, in this case, `utils/`. The `DIR` in `utils/rules.mk` and `heapsort/rules.mk` have different values, but the variable name is the same. In `utils/rules.mk`, `DIR` is `${ROOT}/utils`. In `heapsort/rules.mk`, `DIR` is `${ROOT}/heapsort`.
* `${DIR}C` and `${DIR}CPP` store the `.c` and `.cpp` files in `utils/`. Note that in `utils/rules.mk`, `${DIR}C` is indeed recognized by the `make` program as the variable whose name is `${ROOT}/utilsC`, whereas in `heapsort/rules.mk`, `${DIR}C` is the same as the variable whose name is `${ROOT}/heapsortC`. This is different from `DIR`, which has the same variable name across different `rules.mk`.
* `${DIR}CFLAGS` and `${DIR}CXXFLAGS` store the C and C++ compiling options that are used for all `.o` files in `utils/`, respectively. By default, as in the above sample `utils/rules.mk`, they are the same as the global `CFLAGS` and `CXXFLAGS`, respectively. But the we can modify them to anything we like.
* `${DIR}INCS` store the include directories that are passed to the compiler when compiling source files in `utils/`. By default, it is just the global `INCS`. But we  can modify it. **NOTE**: `${DIR}INCS` is passed to the compiler as is, without any modification. So, for example, we want to have `/opt/fftw/3.4.3/include` to the `include <>` or `include ""` search path, we need to wrtie `${DIR}INCS:=-I/opt/fftw/3.4.3/include` or `${DIR}INCS:=-iquote/opt/fftw/3.4.3/include`. In practice, however, it is most convenient to write a monolithic global `INCS` and leave the `${DIR}INCS` in each `rules.mk` files unchanged.
* The "INTERNAL VARIABLES AND PATTERN RULES" section record the pattern rules that are specific to `utils/`. The pattern rules are slightly different for in-source build and out-of-source build. The `ifeq(OUT_OF_SOURCE,TRUE) (...) else (...) endif` chooses out-of-source build if and only if `OUT_OF_SOURCE` is exactly `TRUE` (case sensitive). Otherwise it will choose in-source build. We have omitted the out-of-source part and will get to it later.
* `OBJ`, `DEP`, and `ASM` are global variables. They are updated with local information. **NOTE**: `${OBJ}:=${OBJ} ...` is not really redefining `OBJ`. Rather, it is adding something new to `OBJ`.

Now, let's explain the core of our method that lies in the following pattern rules:

```
${DIR}/%.o: ${DIR}/%.c
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}
```

Let's start from the obvious. The recipe is quite self-explanatory. It just compile the prerequisite `.c` file to get the target `.o` file. The `.o` file is in the same directory as the `.c` file. So, it is in-source build. The compiling options and include directories are passed using *directory-specific* variables `${DIR}CFALGS` and `${DIR}INCS`, respectively. Next, we wish explain that the pattern rule `${DIR}/%.o: ${DIR}/%.c` will apply to and only apply to `utils/*.c`. So that this pattern becomes completely local to the subdirectory `utils/`.

One may wonder: what's the difference between `%.o: %.c` and `${DIR}/%.o: ${DIR}/%.c`? Indeed, they are very different. For instance, we may have the following two dependencies:

```
utils/utils.o: utils/utils.c

heapsort/heapsort.o: heapsort/heapsort.c
```

The pattern rule `%.o: %.c` will apply to both of them. But in `utils/rules.mk`, because `DIR:=${ROOT}/utils`, `${DIR}/%.o: ${DIR}/%.c` will only apply to the first one. In `heapsort/rules.mk`, because, `DIR:=${ROOT}/heapsort`, `${DIR}/%.o: ${DIR}/%.c` will only apply to the second one. So, such use of pattern rules helps us achieve *directory-specific pattern rules*.

There *may* be a problem of overloading pattern rules. If we specify both `%.o: %.c` and `${DIR}/%.o: ${DIR}/%.c` at the same time, the `make` program may behave strangely. The `make` program on different platforms may have different behaviors in situations with overloaded pattern rules. To make it portable, as well as stable and predictable, we have to make sure that the `make` internal pattern rule `%.o: %.c` is undefined, which is done by the following line in the `root.mk`:

```
.SUFFIXES:
```

### Summary
Our method of writing `Makefile`s

* is a single `Makefile` approach, and 
* completely supports directory-specific pattern rules, and
* supports and can switch between in-source build and out-of-source build very easily, and 
* in the in-source build mode, allows multiple `Makefile`s to work coherently.

The `make` program and `Makefile`s are vast and complicated. Building a project from sources is by itself a good project. People have attempted to address various problems such as modularity, maintainability, and portability. The new method of writing `Makefile`s explained here mainly addresses the modularity and maintainability problems. We sincerely hope that it will be helpful.

## The zmake script
The `zmake` script can generate `root.mk`, `rules.mk` files, and `Makefile`s.
**NOTE**: The out-of-source build function is removed from the the `zmake` script. The old `zmake` that supports out-of-source build is renamed to `zmake2`.

### List of requirement
* POSIX OS.
* Only support C/C++ projects.
* A working python3 interpreter.

### How to get help from the command line?
Make sure that the `zmake` script is included in `$PATH`, type

	zmake -h
	
Then you should see:

```
usage: zmake [-h] [-f | -s | -d] [-r | -b | -R] [-g] [-o TARGET] [dir]

Generating module files for constructing a single Makefile

positional arguments:
  dir                directory of source files (.)

optional arguments:
  -h, --help         show this help message and exit
  -f, --force        force overwriting existing files (False)
  -s, --skip         skip any existing file (False)
  -d, --delete       recursively delete all .mk files in [dir] (False)
  -r, --root-only    generate [dir]/root.mk (False)
  -b, --branch-only  generate [dir]/rules.mk (False)
  -R, --recursive    recursively generate rules.mk's (False)

Makefile:
  -g, --makefile     generate a Makefile (False)
  -o TARGET          output the Makefile to TARGET (./Makefile)
```


### Generate root.mk
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

### Generate `rules.mk` recursively
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

### Generate only one rules.mk
Sometimes it might be desirable to generate the `rules.mk` for a single subdirectory. The `zmake` script can do this too.

In terminal, in the root directory of `zmake`, type

	rm demo/utils/rules.mk
	zmake -b demo/utils
	
You will see

	generated demo/utils/rules.mk
	
This time, the `-b/--branch-only` options instructs the `zmake` script to only generate one `rules.mk` file.

### Delete `.mk` files

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

Open the `demo/heapsort/Makefile` with your favorite editor, uncomment the following lines (delete the `#` at the start of each line)

```
#test_heapsort.exe: ${ROOT}/heapsort/heapsort.o \
	#${ROOT}/heapsort/test_heapsort.o \
	#${ROOT}/utils/utils.o
```
Save the modified `Makefile`. Back in terminal, type

	cd demo/heapsort
	make test_heapsort.exe

You will see an executable `test_heapsort.exe` in the `demo/heapsort/` directory. Type

	./test_heapsort.exe

You will see something interesting.

**NOTE**: The `-f/--force` and `-s/--skip` options work with `-g/--makefile` too.

### Generate out-of-source build `Makefile`s
To get out-of-source `Makefile`s, we can either uncomment the following line in the in-source `Makefile`s:

	#OUT_OF_SOURCE:=TRUE# TRUE = out-of-source build, otherwise = in-source build
	#BUILD:=.# out-of-source build directory, unused in in-source build mode

or use the `--out-of-source` option with `-g/--makefile`. These two methods will give exactly the same result.

**NOTE**: It is strongly recommended to set `BUILD:=.` for out-of-source `Makefile`s. This will simplify the listing of linking rules.