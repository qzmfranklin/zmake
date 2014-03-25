zmake
=====
Tools for generating modules for constructing GNU Makefiles.

## What's in a Makefile?
### GNU make
The GNU make utility and Makefiles has been the de facto standard building tool on POSIX systems for decades. Almost all other building tools such as the CMake and the GNU autotools all build on the make utility.
### Recursive make
The GNU make utility takes a single, complete directed-acyclic-graph (DAG) as input, parses the DAG, travserses it, then arranges for proper target: prerequisite rules to take action. Many large projects tend to use a technique called recursive make because it is perhaps the easiest way one can think of. 

However, Peter Miller, in his famous [*Recursive Make Considered Harmful*](http://aegis.sourceforge.net/auug97.pdf), pointed out that using recursive make breaks the completeness of the DAG. Much manual work must be done to complete the DAG. The resulting set of Makefiles is usually very hard to read, modify, and maintain.
### Single Makefile approach
Peter Miller was right in pointing out the source of problem in using recursive make. He also proposed an alternative implementation using a single Makefile. [His implementation](http://evbergen.home.xs4all.nl/nonrecursive-make.html) avoided using recursive make by keeping track of a global stack depth variable `sp`, a directory-specific stack `dirstack_(sp)`, and a stack pointer `d`.

However, Peter Miller's implementation is recursive in nature and is vulnerable to mistakes as it involves manual tracking of stacks and stack pointers in a non-trivial way.

Can we write a Makefile that is non-recursive in nature and still preserves the directory-specific modularity? Yes, we can.
### Good single Makefile approach
The core idea is to use directory-specific pattern rules.
For example, only the `.cpp` files in `heapsort/` are automatically matched to `%.o: heapsort/%.cpp`. `%.o: heapsort/%.cpp` does not apply to `utils.o: utils/utils.cpp`.

[TODO: Explain the modules and Makefile]

See `demo/` for more information.

## What does zmake do?
In short, zmake generates modules (`root.mk` and `branch.mk`'s) that are used to construct Makefiles. Makefiles constructed this way fully support directory-specific pattern rules, and are single-Makefile projects.

The zmake script can also generate Makefiles from existing `root.mk` and `branch.mk`'s, and can recursively clean all `root.mk` and `branch.mk`.

## How to use zmake?
### Generate `.mk` files
In a terminal, `cd` into the root directory of zmake. Type

	cd bin
	./zmake demo

The positional argument `demo` is the root directory of all the source files. The zmake script recursively scans the `demo/` directory for `.c` and `.cpp` files to generates various `.mk` files. You will see the following message:

	generated demo/root.mk
	100 generated demo/branch.mk
	101 generated demo/diffpar/branch.mk
	102 generated demo/heapsort/branch.mk
	103 generated demo/utils/branch.mk

The number 100,101,102,103 are internal numbers used to distinguish different directories. As long as all of the numbers are **distinct**, we are in good shape.

### Generate Makefiles
Various `.mk` files are generated. The next step is to include proper `.mk` files to a single Makefile. See `demo/src` (not `bin/demo/src`) for sample Makefiles.

In case we need to generate multiple Makefiles, the zmake script can help. Here it is. Type

	./zmake demo -g

The first argument `demo` is still the root directory of the source tree. The `-g` flag tells zmake to generate a Makefile, instead of generating `.mk` files. You will see the following message:

	generated ./Makefile
	
Type

	more Makefile

You should see the first line is

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
# Include all branches herebranch.mk
-include ${ROOT}/diffpar/branch.mk
-include ${ROOT}/heapsort/branch.mk
-include ${ROOT}/utils/branch.mk
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

## How does zmake work?
[TODO: explain the design of zmake (really neccessary?)]

## What more?
[TODO: ]