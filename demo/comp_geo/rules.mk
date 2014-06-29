#  THIS DIRECTORY
DIR00008:=${ROOT}/comp_geo
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR00008}C:=
${DIR00008}CPP:=convex_hull.cpp gen2dpts.cpp graham_scan.cpp geo_utils.cpp \
	test_convex_hull.cpp
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR00008}CFLAGS:=${CFLAGS}
${DIR00008}CXXFLAGS:=${CXXFLAGS}
${DIR00008}INCS:=${INCS}
${DIR00008}LIBS:=${LIBS}

DEP+=${${DIR00008}CPP:%.cpp=${DIR00008}/%.d} ${${DIR00008}C:%.c=${DIR00008}/%.d} 
OBJ+=${${DIR00008}CPP:%.cpp=${DIR00008}/%.o} ${${DIR00008}C:%.c=${DIR00008}/%.o} 
ASM+=${${DIR00008}CPP:%.cpp=${DIR00008}/%.s} ${${DIR00008}C:%.c=${DIR00008}/%.s} 

${DIR00008}/%.o: ${DIR00008}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR00008}CFLAGS} ${${DIR00008}INCS}
${DIR00008}/%.s: ${DIR00008}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR00008}CFLAGS} ${${DIR00008}INCS}

${DIR00008}/%.o: ${DIR00008}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR00008}CXXFLAGS} ${${DIR00008}INCS}
${DIR00008}/%.s: ${DIR00008}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR00008}CXXFLAGS} ${${DIR00008}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR00008}/%.o
	${CXX} -o $@ $^ ${${DIR00008}LIBS}
