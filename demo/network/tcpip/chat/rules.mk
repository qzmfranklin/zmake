#  THIS DIRECTORY
DIRc07e84ba35c9ecbde1c2adceccc766d7:=${ROOT}/network/tcpip/chat
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRc07e84ba35c9ecbde1c2adceccc766d7}C:=
${DIRc07e84ba35c9ecbde1c2adceccc766d7}CPP:=client.cpp server.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRc07e84ba35c9ecbde1c2adceccc766d7}CFLAGS:=${CFLAGS}
${DIRc07e84ba35c9ecbde1c2adceccc766d7}CXXFLAGS:=${CXXFLAGS}
${DIRc07e84ba35c9ecbde1c2adceccc766d7}INCS:=${INCS}
${DIRc07e84ba35c9ecbde1c2adceccc766d7}LIBS:=${LIBS} -pthread

DEP+=${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CPP:%.cpp=${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.d} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}C:%.c=${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.d} 
OBJ+=${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CPP:%.cpp=${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.o} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}C:%.c=${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.o} 
ASM+=${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CPP:%.cpp=${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.s} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}C:%.c=${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.s} 

${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.o: ${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}INCS}
${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.s: ${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}INCS}

${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.o: ${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CXXFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}INCS}
${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.s: ${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}CXXFLAGS} ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRc07e84ba35c9ecbde1c2adceccc766d7}/%.o
	${CXX} -o $@ $^ ${${DIRc07e84ba35c9ecbde1c2adceccc766d7}LIBS}
