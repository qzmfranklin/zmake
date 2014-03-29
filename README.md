zmake
=====
A new method of writing `Makefile`s.

## What's in a `Makefile`?
### The GNU `make` program and `Makefile`s
The  `make` program takes a file, typically called `Makefile`, from the command line. The `Makefile` provides information on dependencies in the form of `target: prerequisite(s)`. There is usually also a `recipe` associated with each `target`. If the `target` is older than any of its `prerequisite`s, the `make` program will invoke the `recipe` associated with the `target`.

The dependencies in a project will form a *directed acyclic graph* (DAG). The `make` program relies on the DAG to function properly.

`Makefile`s are old, vast, and complicated. Writing good `Makefile`s is not easy.

To illustrate various approaches, let's first introduce a sample project.


#### Sample project

Suppose we have a C++ project with the following source tree:

```
src/
├── heapsort/
│   ├── heapsort.cpp
│   ├── heapsort.h
│   └── test_heapsort.cpp
└── utils/
    ├── test_utils.cpp
    ├── utils.cpp
    └── utils.h
```
Suppose there are two executables to build: `test_heapsort.exe` and `test_utils.exe`. And we want to use `-O3` and `-g` as the compiling flags for `heapsort` and `utils`, respectively. We also want to link `test_heapsort.o` with `-lm` but link `test_utils.o` with no extra libraries. 

This is a small project. But it is complicated enough to demonstrate our points.


### The old single `Makefile` approach

Build the sample project using a single monolithic `Makefile` (`demo0/Makefile`):

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

The problem with the traditional single `Makefile` approach, i.e., writing a monolithic `Makefile` for a large project, has some major disadavantages.

* Programmers have to manually list all the dependencies.  As the project grows larger and larger, it would at some point become practically impossible to read, modify, and maintain.
* In theory, pattern rules such as `.o .c` and `%.o : %.c` can be used to reduce the amount of manual work. But in practice, such pattern rules do not permit ***directory-specific*** recipes, which renders them almost useless for large projects that have different compiling and building rules for different directories.

To counter these issues, especially as an effort to achieve the modularity at the *directory level*, people found a new way of writing `Makefiles` - using *recursive `make`*.

### Recursive make considered harmful

Recursive `make` has good modularity. But, as Peter Miller pointed out in his famous [*Recursive make considered harmful*](http://aegis.sourceforge.net/auug97.pdf), recursive `make` breaks the completeness of the DAG. This has several consequencies:

* The incompleteness of the DAG means that the `make` program will build too little. 
* Manually "patching" the DAG would usually result in an over-complete `Makefile` that builds too much.
* Parallel `make` using the `-j` flag becomes very tricky, if not at all impossible.
* Indeed, `Makefile`s written using recursive `make` are hard to maintain, too.

Understandably, these problems are hard to solve within the scope of the recursive `make`. So, Peter Miller came to the conclusion that recursive `make` is harmful and the right way of writing `Makefile`s is to write a single `Makefile` the builds the entire project. He referred readers to the implementation of Emile van Bergen for a new solution.

### The single `Makefile` solution due to Emile van Bergen
The implementation due to [Emile](http://evbergen.home.xs4all.nl/nonrecursive-make.html) is a single `Makefile` approach that has modularity at the directory level. 
or what Emile did. But we will start from this plain version `Makefile`.

#### Emile's solution





### Finally: a good single `Makefile` solution






## What does zmake do?
In short, zmake generates modules (`root.mk` and `rules.mk`'s) that are used to construct Makefiles. Makefiles constructed this way fully support directory-specific pattern rules, and are single-Makefile projects.

The zmake script can also generate Makefiles from existing `root.mk` and `rules.mk`'s, and can recursively clean all `root.mk` and `rules.mk`.

## How to use zmake?
### Can I use zmake?
Here is the list of requirements:

* POSIX OS.
* Only support C/C++ projects.
* Must have a working python3 interpreter.

Maybe in the future I can/will extend the coverage, if there is demand.

### How to get help from command line?
This always works:

	zmake -h
	
Then you will see the usage information:

```
usage: zmake [-h] [-f] [-u | -d | -g] [-o TARGET] [-n N]
             [--in-source | --out-of-source]
             [root]

[TODO: ]
Generating module files for constructing a single Makefile

positional arguments:
  root                  root directory of source files (.)

optional arguments:
  -h, --help            show this help message and exit
  -f, --force           force overwrite (False)
  -u, --update          skip existing .mk files (False)
  -d, --delete          recursively delete all root.mk and rules.mk's (False)
  -g, --generate-makefile
                        generate a Makefile (False)
  -o TARGET             output the Makefile to TARGET (./Makefile)
  -n N, --max-recursive-depth N
                        maximal depth for upwards search when creating
                        Makefiles (3)
  --in-source           the generated Makefile uses in-source build (True)
  --out-of-source       the generated Makefile uses out-of-source build

```
### Generate `.mk` files
In a terminal, `cd` into the root directory of zmake. Type

	cd bin
	./zmake demo

The positional argument `demo` is the root directory of all the source files. The zmake script recursively scans the `demo/` directory for `.c` and `.cpp` files to generates various `.mk` files. You will see the following message:

	generated demo/root.mk
	100 generated demo/rules.mk
	101 generated demo/diffpar/rules.mk
	102 generated demo/heapsort/rules.mk
	103 generated demo/utils/rules.mk

The number 100,101,102,103 are internal numbers used to distinguish different directories. As long as all of the numbers are **distinct**, we are in good shape.

### Generate Makefiles
Various `.mk` files are generated. The next step is to include proper `.mk` files to a single Makefile. See `demo/src` (not `bin/demo/src`) for sample Makefiles.

In case we need to generate multiple Makefiles, the zmake script can help. Here it is. Type

	./zmake demo -g

The first argument `demo` is still the root directory of the source tree. The `-g` flag tells zmake to generate a Makefile, instead of generating `.mk` files. You will see the following message:

	generated ./Makefile
	
Type

	more Makefile

The first line should be

	#  Sample Makefile for in-source build

You may use `-o TARGET` to output to different pathnames. For example:

	./zmake demo -g -o ../Makefile2
	
This time, a Makefile with a different path and/or name is generated. You will see the following message:

	generated ../Makefile2

### How to use the generated Makefiles?
Your `./Makefile` should have the following content:

```
#  Sample Makefile for in-source build
################################################################################
#QUIET:=@# uncomment this line to silence
ROOT :=#your own directory#
# Include the top-level .mk file, i.e., the root.mk
-include ${ROOT}/root.mk
# Include all branches hererules.mk
-include ${ROOT}/diffpar/rules.mk
-include ${ROOT}/heapsort/rules.mk
-include ${ROOT}/utils/rules.mk
# Then include all dependency files
-include ${DEP}
################################################################################ 
#	LIST LINKING RULES
# Sample linking rules:
# The redundancy in ${ROOT} is a must. There is no easy way around.
#test_heapsort.exe: ${ROOT}/heapsort/heapsort.o \
	#${ROOT}/heapsort/test_heapsort.o \
	#${ROOT}/utils/utils.o
```
Add all your own linking rules after `LIST LINKING RULES`. Sample linking rules and brief explanation is already included in the Makefile.

**NOTE**: The `${ROOT}/` prefix to every single `.o` file in the `*.exe: *.o ...` rules is a must. This redundancy is due to duplicate pathnames. i.e., `demo/heapsort/..` and `demo` are recognized as two distinct pathnames in the GNU make utility.
	

### In-source build vs. out-of-source build
By default, the flag `-g` directs the zmake script to generate a Makefile that builds in-source. You may use `--out-of-source` (pairs with `--in-source`]) to get Makefile that builds out-of-source. For example, type

	./zmake -g --out-of-source demo

will generate a Makefile starting with

	#  Sample Makefile for out-of-source build

**Note** that out-of-source Makefiles and in-source Makefiles use different link rules. For details and instructions, see the actually generated Makefiles.

### What should I do if I modified the sources after `.mk` files and Makefiles were generated?
In short, `zmake` will take care of compiling and you will have to take care of linking.

#### I have only added/removed some `.c` and `.cpp` files, but have not changed the structure of the directories
Then a simple

	./zmake [root] -f

will do the work. The flag `-f/--force` forces overwriting existing `.mk` files. If you do not specify `-f/--force`, you will be prompted

	bin/demo/diffpar/rules.mk already exists, overwrite (y/n/q)?
	
so you can decide whether to overwrite on each file.


#### I have changed the directory structure
This might seem to be very challenging. But all you have to do is regenerate all the `.mk` files and then update the linking rules in Makefiles. The `zmake` script can do the update for you.
The following command

	./zmake --update
 
 will update all `.mk` files.
 
 Of course, you have to update the linking rules in Makefiles manually. There is no easy way around.

## How to delete all the `.mk` files?
If your codes are in `[root]` directory, then

	./zmake --delete [root]
	
will delete all the `.mk` files. 

For example. In the `bin` directory, type 

	./zmake --delete demo
	
A sample message is:

```
deleted demo/root.mk
deleted demo/diffpar/rules.mk
deleted demo/heapsort/rules.mk
deleted demo/utils/rules.mk
```

## Some notes on usage
* The default value of `[root]` is `.`, which is the current working directory of the `zmake` script.
* `-u,-d,-g` are mutually exclusive. You may only specify at most one of them at a time. You may try to specify more than one, the python3 interpreter will report error.
* `./zmake -g` will automatically search for `root.mk` from the `[root]` directory, which default value is the current directory `./`. If `root.mk` is not in `[root]`, `zmake` will search upwards, i.e., search `[root]/..` and `[root]/../..` and so on. By default, the maximal recursive search depth is 3. You may change this behavior by specifying `-n N, --max-recursive-depth N`, where `N` is the depth.
* `--in-source` and `--out-of-source` are mutually exclusive.
* `--force` and `--update` are *not* mutually exclusive. `--update` overwrite `--force`.
* You may specify `-o TARGET` without `-g`. But in that case `-o TARGET` is just not used.

## Other projects that are using `zmake`
* [spoons](https://github.com/qzmfranklin/spoons.git)
* (more to come)

## What more?
I am considering adding support for `Fortran` source files. That may help more people migrate to `zmake`.