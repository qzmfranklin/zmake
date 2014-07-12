#  THIS DIRECTORY
DIRa36ebe408e3ccc8eb7ea1d969ef946d4:=${ROOT}/comp_geo
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}C:=
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CPP:=gen2dpts.cpp geo_utils.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CFLAGS:=${CFLAGS}
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CXXFLAGS:=${CXXFLAGS}
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}INCS:=${INCS}
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}LIBS:=${LIBS}

DEP+=${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CPP:%.cpp=${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.d} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}C:%.c=${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.d} 
OBJ+=${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CPP:%.cpp=${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.o} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}C:%.c=${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.o} 
ASM+=${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CPP:%.cpp=${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.s} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}C:%.c=${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.s} 

${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.o: ${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}INCS}
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.s: ${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}INCS}

${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.o: ${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CXXFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}INCS}
${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.s: ${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}CXXFLAGS} ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}/%.o
	${CXX} -o $@ $^ ${${DIRa36ebe408e3ccc8eb7ea1d969ef946d4}LIBS}
