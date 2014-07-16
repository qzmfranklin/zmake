#  THIS DIRECTORY
DIR00012:=${ROOT}/rte2dvis/solver
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00012}C:=
${DIR00012}CPP:=rte2dvisv1.cpp solver_v1.cpp solver_v2.cpp test_solver_v2.cpp test_solver_v1.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00012}CFLAGS:=${CFLAGS}
${DIR00012}CXXFLAGS:=${CXXFLAGS}
${DIR00012}INCS:=${INCS}
${DIR00012}LIBS:=${LIBS}

DEP+=${${DIR00012}CPP:%.cpp=${DIR00012}/%.d} ${${DIR00012}C:%.c=${DIR00012}/%.d} 
OBJ+=${${DIR00012}CPP:%.cpp=${DIR00012}/%.o} ${${DIR00012}C:%.c=${DIR00012}/%.o} 
ASM+=${${DIR00012}CPP:%.cpp=${DIR00012}/%.s} ${${DIR00012}C:%.c=${DIR00012}/%.s} 

${DIR00012}/%.o: ${DIR00012}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00012}CFLAGS} ${${DIR00012}INCS}
${DIR00012}/%.s: ${DIR00012}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00012}CFLAGS} ${${DIR00012}INCS}

${DIR00012}/%.o: ${DIR00012}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00012}CXXFLAGS} ${${DIR00012}INCS}
${DIR00012}/%.s: ${DIR00012}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00012}CXXFLAGS} ${${DIR00012}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00012}/%.o
	${CXX} -o $@ $^ ${${DIR00012}LIBS}
