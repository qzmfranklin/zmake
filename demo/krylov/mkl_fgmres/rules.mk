#  THIS DIRECTORY
DIRfb159fde1affb8870954e81852674ebf:=${ROOT}/krylov/mkl_fgmres
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRfb159fde1affb8870954e81852674ebf}C:=
${DIRfb159fde1affb8870954e81852674ebf}CPP:=test_mkl_solvers.cpp 

ifeq ($(shell uname),Darwin)
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
MKLROOT:=$(shell echo $$MKLROOT)
${DIRfb159fde1affb8870954e81852674ebf}CFLAGS:=${CFLAGS}
${DIRfb159fde1affb8870954e81852674ebf}CXXFLAGS:=${CXXFLAGS} \
	 -m64 -I$(MKLROOT)/include
${DIRfb159fde1affb8870954e81852674ebf}INCS:=${INCS}
${DIRfb159fde1affb8870954e81852674ebf}LIBS:=${LIBS} \
	 -L$(MKLROOT)/lib -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm
endif

ifeq ($(shell uname),Linux)
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
MKLROOT:=$(shell echo $$MKLROOT)
${DIRfb159fde1affb8870954e81852674ebf}CFLAGS:=${CFLAGS}
${DIRfb159fde1affb8870954e81852674ebf}CXXFLAGS:=${CXXFLAGS} \
	 -m64 -I$(MKLROOT)/include
${DIRfb159fde1affb8870954e81852674ebf}INCS:=${INCS}
${DIRfb159fde1affb8870954e81852674ebf}LIBS:=${LIBS} \
	 -L$(MKLROOT)/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm
endif

DEP+=${${DIRfb159fde1affb8870954e81852674ebf}CPP:%.cpp=${DIRfb159fde1affb8870954e81852674ebf}/%.d} ${${DIRfb159fde1affb8870954e81852674ebf}C:%.c=${DIRfb159fde1affb8870954e81852674ebf}/%.d} 
OBJ+=${${DIRfb159fde1affb8870954e81852674ebf}CPP:%.cpp=${DIRfb159fde1affb8870954e81852674ebf}/%.o} ${${DIRfb159fde1affb8870954e81852674ebf}C:%.c=${DIRfb159fde1affb8870954e81852674ebf}/%.o} 
ASM+=${${DIRfb159fde1affb8870954e81852674ebf}CPP:%.cpp=${DIRfb159fde1affb8870954e81852674ebf}/%.s} ${${DIRfb159fde1affb8870954e81852674ebf}C:%.c=${DIRfb159fde1affb8870954e81852674ebf}/%.s} 

${DIRfb159fde1affb8870954e81852674ebf}/%.o: ${DIRfb159fde1affb8870954e81852674ebf}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}CFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}INCS}
${DIRfb159fde1affb8870954e81852674ebf}/%.s: ${DIRfb159fde1affb8870954e81852674ebf}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}CFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}INCS}

${DIRfb159fde1affb8870954e81852674ebf}/%.o: ${DIRfb159fde1affb8870954e81852674ebf}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}CXXFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}INCS}
${DIRfb159fde1affb8870954e81852674ebf}/%.s: ${DIRfb159fde1affb8870954e81852674ebf}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}CXXFLAGS} ${${DIRfb159fde1affb8870954e81852674ebf}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRfb159fde1affb8870954e81852674ebf}/%.o
	${CXX} -o $@ $^ ${${DIRfb159fde1affb8870954e81852674ebf}LIBS}
