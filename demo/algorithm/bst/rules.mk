#  THIS DIRECTORY
DIRdfc4f92644e6fad0c3c43f2c76b656f1:=${ROOT}/algorithm/bst
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}C:=
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CPP:=bst.cpp gtest_bst.cpp
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CFLAGS:=${CFLAGS}
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CXXFLAGS:=${CXXFLAGS}
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}INCS:=${INCS}
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}LIBS:=${LIBS}

DEP+=${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CPP:%.cpp=${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.d} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}C:%.c=${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.d} 
OBJ+=${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CPP:%.cpp=${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.o} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}C:%.c=${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.o} 
ASM+=${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CPP:%.cpp=${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.s} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}C:%.c=${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.s} 

${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.o: ${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}INCS}
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.s: ${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}INCS}

${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.o: ${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CXXFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}INCS}
${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.s: ${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}CXXFLAGS} ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRdfc4f92644e6fad0c3c43f2c76b656f1}/%.o
	${CXX} -o $@ $^ ${${DIRdfc4f92644e6fad0c3c43f2c76b656f1}LIBS}
