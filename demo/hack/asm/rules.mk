#  THIS DIRECTORY
DIR9c7c1641877e775a1db1d632141ccd98:=${ROOT}/hack/asm
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR9c7c1641877e775a1db1d632141ccd98}C:=helloworldc.c
${DIR9c7c1641877e775a1db1d632141ccd98}CPP:= 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR9c7c1641877e775a1db1d632141ccd98}CFLAGS:=${CFLAGS}
${DIR9c7c1641877e775a1db1d632141ccd98}CXXFLAGS:=${CXXFLAGS}
${DIR9c7c1641877e775a1db1d632141ccd98}INCS:=${INCS}
${DIR9c7c1641877e775a1db1d632141ccd98}LIBS:=${LIBS}

DEP+=${${DIR9c7c1641877e775a1db1d632141ccd98}CPP:%.cpp=${DIR9c7c1641877e775a1db1d632141ccd98}/%.d} ${${DIR9c7c1641877e775a1db1d632141ccd98}C:%.c=${DIR9c7c1641877e775a1db1d632141ccd98}/%.d} 
OBJ+=${${DIR9c7c1641877e775a1db1d632141ccd98}CPP:%.cpp=${DIR9c7c1641877e775a1db1d632141ccd98}/%.o} ${${DIR9c7c1641877e775a1db1d632141ccd98}C:%.c=${DIR9c7c1641877e775a1db1d632141ccd98}/%.o} 
ASM+=${${DIR9c7c1641877e775a1db1d632141ccd98}CPP:%.cpp=${DIR9c7c1641877e775a1db1d632141ccd98}/%.s} ${${DIR9c7c1641877e775a1db1d632141ccd98}C:%.c=${DIR9c7c1641877e775a1db1d632141ccd98}/%.s} 

${DIR9c7c1641877e775a1db1d632141ccd98}/%.o: ${DIR9c7c1641877e775a1db1d632141ccd98}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}CFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}INCS}
${DIR9c7c1641877e775a1db1d632141ccd98}/%.s: ${DIR9c7c1641877e775a1db1d632141ccd98}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}CFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}INCS}

${DIR9c7c1641877e775a1db1d632141ccd98}/%.o: ${DIR9c7c1641877e775a1db1d632141ccd98}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}CXXFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}INCS}
${DIR9c7c1641877e775a1db1d632141ccd98}/%.s: ${DIR9c7c1641877e775a1db1d632141ccd98}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}CXXFLAGS} ${${DIR9c7c1641877e775a1db1d632141ccd98}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR9c7c1641877e775a1db1d632141ccd98}/%.o
	${CXX} -o $@ $^ ${${DIR9c7c1641877e775a1db1d632141ccd98}LIBS}
