################################################################################
#	PROJECT-WIDE COMMON COMPILING FLAGS 
CC		:=icc
CFLAGS 		:=-O3							\
		-Wall							\
		-std=c99						\
		-Wno-deprecated

CXX		:=icpc
CXXFLAGS	:=-O3							\
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
DEPFLAGS	:=-MMD -MP# preprocessor generates .d files
ASMFLAGS	:=-S -fsource-asm# source code commented assembly code 
.SUFFIXES:
################################################################################
# 	OUT-OF-SOURCE PATTERN RULES
ifeq (OUT_OF_SOURCE,TRUE)
${BUILD}/%.exe: ${BUILD}/%.o
	${QUIET}${CXX} -o $@ $^ ${LIBS} 
endif
