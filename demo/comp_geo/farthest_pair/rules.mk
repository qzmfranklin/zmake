#  THIS DIRECTORY
DIR00009:=${ROOT}/comp_geo/farthest_pair
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00009}C:=
${DIR00009}CPP:=convex_hull.cpp gen2dpts.cpp test_convex_hull.cpp graham_scan.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00009}CFLAGS:=${CFLAGS}
${DIR00009}CXXFLAGS:=${CXXFLAGS}
${DIR00009}INCS:=${INCS}
${DIR00009}LIBS:=${LIBS}

DEP+=${${DIR00009}CPP:%.cpp=${DIR00009}/%.d} ${${DIR00009}C:%.c=${DIR00009}/%.d} 
OBJ+=${${DIR00009}CPP:%.cpp=${DIR00009}/%.o} ${${DIR00009}C:%.c=${DIR00009}/%.o} 
ASM+=${${DIR00009}CPP:%.cpp=${DIR00009}/%.s} ${${DIR00009}C:%.c=${DIR00009}/%.s} 

${DIR00009}/%.o: ${DIR00009}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00009}CFLAGS} ${${DIR00009}INCS}
${DIR00009}/%.s: ${DIR00009}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00009}CFLAGS} ${${DIR00009}INCS}

${DIR00009}/%.o: ${DIR00009}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00009}CXXFLAGS} ${${DIR00009}INCS}
${DIR00009}/%.s: ${DIR00009}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00009}CXXFLAGS} ${${DIR00009}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00009}/%.o
	${CXX} -o $@ $^ ${${DIR00009}LIBS}
