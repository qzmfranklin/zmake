#  THIS DIRECTORY
DIRcbb4a3b1cb3c80f6e6ef965a093e4a56:=${ROOT}/algorithm/sort
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}C:=
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CPP:=cmp_sort.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CFLAGS:=${CFLAGS}
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CXXFLAGS:=${CXXFLAGS}
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}INCS:=${INCS}
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}LIBS:=${LIBS}

DEP+=${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CPP:%.cpp=${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.d} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}C:%.c=${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.d} 
OBJ+=${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CPP:%.cpp=${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.o} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}C:%.c=${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.o} 
ASM+=${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CPP:%.cpp=${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.s} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}C:%.c=${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.s} 

${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.o: ${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}INCS}
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.s: ${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}INCS}

${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.o: ${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CXXFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}INCS}
${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.s: ${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}CXXFLAGS} ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}/%.o
	${CXX} -o $@ $^ ${${DIRcbb4a3b1cb3c80f6e6ef965a093e4a56}LIBS}
