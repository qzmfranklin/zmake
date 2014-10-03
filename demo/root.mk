#	PROJECT-WIDE COMMON COMPILING FLAGS 
CC		:=g++
CFLAGS 		:=-O3 -Wshadow -Wno-unused-result -Drestrict=__restrict__ \
	-DNDEBUG -g -fpermissive

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
