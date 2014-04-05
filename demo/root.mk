#	PROJECT-WIDE COMMON COMPILING FLAGS 
CC		:=gcc
CFLAGS 		:=-O3 -Wall -std=c99

CXX		:=g++
CXXFLAGS	:=-O3 -Wall
#       PROJECT-WIDE DEFAULT LINKING LIBRARIES AND INCLUDE DIRECTORIES
INCS		:=-iquote ${ROOT}
LIBS		:=
################################################################################
#		INTERNAL VARIABLES
OBJ		:=# .o files
DEP		:=# .d files
ASM		:=# .s files
DEPFLAGS	:=-MMD -MP# preprocessor generates .d files
ASMFLAGS	:=-S -fsource-asm# source code commented assembly code 
.SUFFIXES:
