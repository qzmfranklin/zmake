#  THIS DIRECTORY
DIR806f3810913589f0ad2933d6ff15b485:=${ROOT}/algorithm/sort/mergesort
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR806f3810913589f0ad2933d6ff15b485}C:=
${DIR806f3810913589f0ad2933d6ff15b485}CPP:=test_mergesort.cpp mergesort.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR806f3810913589f0ad2933d6ff15b485}CFLAGS:=${CFLAGS}
${DIR806f3810913589f0ad2933d6ff15b485}CXXFLAGS:=${CXXFLAGS}
${DIR806f3810913589f0ad2933d6ff15b485}INCS:=${INCS}
${DIR806f3810913589f0ad2933d6ff15b485}LIBS:=${LIBS}

DEP+=${${DIR806f3810913589f0ad2933d6ff15b485}CPP:%.cpp=${DIR806f3810913589f0ad2933d6ff15b485}/%.d} ${${DIR806f3810913589f0ad2933d6ff15b485}C:%.c=${DIR806f3810913589f0ad2933d6ff15b485}/%.d} 
OBJ+=${${DIR806f3810913589f0ad2933d6ff15b485}CPP:%.cpp=${DIR806f3810913589f0ad2933d6ff15b485}/%.o} ${${DIR806f3810913589f0ad2933d6ff15b485}C:%.c=${DIR806f3810913589f0ad2933d6ff15b485}/%.o} 
ASM+=${${DIR806f3810913589f0ad2933d6ff15b485}CPP:%.cpp=${DIR806f3810913589f0ad2933d6ff15b485}/%.s} ${${DIR806f3810913589f0ad2933d6ff15b485}C:%.c=${DIR806f3810913589f0ad2933d6ff15b485}/%.s} 

${DIR806f3810913589f0ad2933d6ff15b485}/%.o: ${DIR806f3810913589f0ad2933d6ff15b485}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}CFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}INCS}
${DIR806f3810913589f0ad2933d6ff15b485}/%.s: ${DIR806f3810913589f0ad2933d6ff15b485}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}CFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}INCS}

${DIR806f3810913589f0ad2933d6ff15b485}/%.o: ${DIR806f3810913589f0ad2933d6ff15b485}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}CXXFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}INCS}
${DIR806f3810913589f0ad2933d6ff15b485}/%.s: ${DIR806f3810913589f0ad2933d6ff15b485}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}CXXFLAGS} ${${DIR806f3810913589f0ad2933d6ff15b485}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR806f3810913589f0ad2933d6ff15b485}/%.o
	${CXX} -o $@ $^ ${${DIR806f3810913589f0ad2933d6ff15b485}LIBS}