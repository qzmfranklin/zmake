#  THIS DIRECTORY
DIR00005:=${ROOT}/comp_geo/farthest_pair
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00005}C:=
${DIR00005}CPP:=convex_hull.cpp graham_scan.cpp test_convex_hull.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00005}CFLAGS:=${CFLAGS}
${DIR00005}CXXFLAGS:=${CXXFLAGS}
${DIR00005}INCS:=${INCS}
${DIR00005}LIBS:=${LIBS}

DEP+=${${DIR00005}CPP:%.cpp=${DIR00005}/%.d} ${${DIR00005}C:%.c=${DIR00005}/%.d} 
OBJ+=${${DIR00005}CPP:%.cpp=${DIR00005}/%.o} ${${DIR00005}C:%.c=${DIR00005}/%.o} 
ASM+=${${DIR00005}CPP:%.cpp=${DIR00005}/%.s} ${${DIR00005}C:%.c=${DIR00005}/%.s} 

${DIR00005}/%.o: ${DIR00005}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00005}CFLAGS} ${${DIR00005}INCS}
${DIR00005}/%.s: ${DIR00005}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00005}CFLAGS} ${${DIR00005}INCS}

${DIR00005}/%.o: ${DIR00005}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00005}CXXFLAGS} ${${DIR00005}INCS}
${DIR00005}/%.s: ${DIR00005}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00005}CXXFLAGS} ${${DIR00005}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00005}/%.o
	${CXX} -o $@ $^ ${${DIR00005}LIBS}
