#  THIS DIRECTORY
DIRc430b1ac6eac4ab083e96273fcfa86da:=${ROOT}/algorithm/str_match/kmp
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRc430b1ac6eac4ab083e96273fcfa86da}C:=
${DIRc430b1ac6eac4ab083e96273fcfa86da}CPP:=kmp_match.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRc430b1ac6eac4ab083e96273fcfa86da}CFLAGS:=${CFLAGS}
${DIRc430b1ac6eac4ab083e96273fcfa86da}CXXFLAGS:=${CXXFLAGS}
${DIRc430b1ac6eac4ab083e96273fcfa86da}INCS:=${INCS}
${DIRc430b1ac6eac4ab083e96273fcfa86da}LIBS:=${LIBS}

DEP+=${${DIRc430b1ac6eac4ab083e96273fcfa86da}CPP:%.cpp=${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.d} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}C:%.c=${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.d} 
OBJ+=${${DIRc430b1ac6eac4ab083e96273fcfa86da}CPP:%.cpp=${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.o} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}C:%.c=${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.o} 
ASM+=${${DIRc430b1ac6eac4ab083e96273fcfa86da}CPP:%.cpp=${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.s} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}C:%.c=${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.s} 

${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.o: ${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}CFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}INCS}
${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.s: ${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}CFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}INCS}

${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.o: ${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}CXXFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}INCS}
${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.s: ${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}CXXFLAGS} ${${DIRc430b1ac6eac4ab083e96273fcfa86da}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRc430b1ac6eac4ab083e96273fcfa86da}/%.o
	${CXX} -o $@ $^ ${${DIRc430b1ac6eac4ab083e96273fcfa86da}LIBS}
