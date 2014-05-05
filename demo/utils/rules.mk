#  THIS DIRECTORY
DIR00003:=${ROOT}/utils
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00003}C:=
${DIR00003}CPP:=test_timestat.cpp test_table.cpp test_stat.cpp utils.cpp Table.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00003}CFLAGS:=${CFLAGS}
${DIR00003}CXXFLAGS:=${CXXFLAGS}
${DIR00003}INCS:=${INCS}
${DIR00003}LIBS:=${LIBS}

DEP+=${${DIR00003}CPP:%.cpp=${DIR00003}/%.d} ${${DIR00003}C:%.c=${DIR00003}/%.d} 
OBJ+=${${DIR00003}CPP:%.cpp=${DIR00003}/%.o} ${${DIR00003}C:%.c=${DIR00003}/%.o} 
ASM+=${${DIR00003}CPP:%.cpp=${DIR00003}/%.s} ${${DIR00003}C:%.c=${DIR00003}/%.s} 

${DIR00003}/%.o: ${DIR00003}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00003}CFLAGS} ${${DIR00003}INCS}
${DIR00003}/%.s: ${DIR00003}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00003}CFLAGS} ${${DIR00003}INCS}

${DIR00003}/%.o: ${DIR00003}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00003}CXXFLAGS} ${${DIR00003}INCS}
${DIR00003}/%.s: ${DIR00003}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00003}CXXFLAGS} ${${DIR00003}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00003}/%.o
	${CXX} -o $@ $^ ${${DIR00003}LIBS}
