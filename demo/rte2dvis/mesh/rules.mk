#  THIS DIRECTORY
DIR00013:=${ROOT}/rte2dvis/mesh
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00013}C:=
${DIR00013}CPP:=test_msh_ray_tracing.cpp msh_ray_tracing.cpp dumpmsh.cpp test_mshio.cpp mshio.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00013}CFLAGS:=${CFLAGS}
${DIR00013}CXXFLAGS:=${CXXFLAGS}
${DIR00013}INCS:=${INCS}
${DIR00013}LIBS:=${LIBS}

DEP+=${${DIR00013}CPP:%.cpp=${DIR00013}/%.d} ${${DIR00013}C:%.c=${DIR00013}/%.d} 
OBJ+=${${DIR00013}CPP:%.cpp=${DIR00013}/%.o} ${${DIR00013}C:%.c=${DIR00013}/%.o} 
ASM+=${${DIR00013}CPP:%.cpp=${DIR00013}/%.s} ${${DIR00013}C:%.c=${DIR00013}/%.s} 

${DIR00013}/%.o: ${DIR00013}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00013}CFLAGS} ${${DIR00013}INCS}
${DIR00013}/%.s: ${DIR00013}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00013}CFLAGS} ${${DIR00013}INCS}

${DIR00013}/%.o: ${DIR00013}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00013}CXXFLAGS} ${${DIR00013}INCS}
${DIR00013}/%.s: ${DIR00013}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00013}CXXFLAGS} ${${DIR00013}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00013}/%.o
	${CXX} -o $@ $^ ${${DIR00013}LIBS}
