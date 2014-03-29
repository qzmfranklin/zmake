################################################################################
#  THIS DIRECTORY
DIR:=${ROOT}/heapsort
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR}C:=
${DIR}CPP:=heapsort.cpp test_heapsort.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR}CFLAGS:=${CFLAGS}
${DIR}CXXFLAGS:=${CXXFLAGS}
${DIR}INCS:=${INCS}
################################################################################ 
#		INTERNAL VARIABLES AND PATTERN RULES
ifeq (${OUT_OF_SOURCE},TRUE)
# out-of-source build
# Add local targets to global variables
DEP:=${DEP} ${${DIR}CPP:%.cpp=${DIR}/%.d} ${${DIR}C:%.c=${DIR}/%.d} 
OBJ:=${OBJ} ${${DIR}CPP:%.cpp=${DIR}/%.o} ${${DIR}C:%.c=${DIR}/%.o} 
ASM:=${ASM} ${${DIR}CPP:%.cpp=${DIR}/%.s} ${${DIR}C:%.c=${DIR}/%.s} 
# C sources
${BUILD}/%.o: ${DIR}/%.c
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}
${BUILD}/%.s: ${DIR}/%.c
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}

# C++ sources
${BUILD}/%.o: ${DIR}/%.cpp
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS}
${BUILD}/%.s: ${DIR}/%.cpp
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS}
else
# in-place build
# Add local targets to global variables
DEP:=${DEP} ${${DIR}CPP:%.cpp=${DIR}/%.d} ${${DIR}C:%.c=${DIR}/%.d} 
OBJ:=${OBJ} ${${DIR}CPP:%.cpp=${DIR}/%.o} ${${DIR}C:%.c=${DIR}/%.o} 
ASM:=${ASM} ${${DIR}CPP:%.cpp=${DIR}/%.s} ${${DIR}C:%.c=${DIR}/%.s} 

# C sources
${DIR}/%.o: ${DIR}/%.c
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}
${DIR}/%.s: ${DIR}/%.c
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS}

# C++ sources
${DIR}/%.o: ${DIR}/%.cpp
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS}
${DIR}/%.s: ${DIR}/%.cpp
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR}/%.o
	${QUIET}${CXX} -o $@ $^ ${LIBS}
endif 
