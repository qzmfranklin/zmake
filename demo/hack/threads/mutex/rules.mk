#  THIS DIRECTORY
DIR1dcd86b038d4a91af743cece9ed52d7f:=${ROOT}/hack/threads/mutex
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR1dcd86b038d4a91af743cece9ed52d7f}C:=
${DIR1dcd86b038d4a91af743cece9ed52d7f}CPP:=dynamic.cpp static.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR1dcd86b038d4a91af743cece9ed52d7f}CFLAGS:=${CFLAGS}
${DIR1dcd86b038d4a91af743cece9ed52d7f}CXXFLAGS:=${CXXFLAGS}
${DIR1dcd86b038d4a91af743cece9ed52d7f}INCS:=${INCS}
${DIR1dcd86b038d4a91af743cece9ed52d7f}LIBS:=${LIBS}

DEP+=${${DIR1dcd86b038d4a91af743cece9ed52d7f}CPP:%.cpp=${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.d} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}C:%.c=${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.d} 
OBJ+=${${DIR1dcd86b038d4a91af743cece9ed52d7f}CPP:%.cpp=${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.o} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}C:%.c=${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.o} 
ASM+=${${DIR1dcd86b038d4a91af743cece9ed52d7f}CPP:%.cpp=${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.s} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}C:%.c=${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.s} 

${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.o: ${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}CFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}INCS}
${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.s: ${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}CFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}INCS}

${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.o: ${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}CXXFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}INCS}
${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.s: ${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}CXXFLAGS} ${${DIR1dcd86b038d4a91af743cece9ed52d7f}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR1dcd86b038d4a91af743cece9ed52d7f}/%.o
	${CXX} -o $@ $^ ${${DIR1dcd86b038d4a91af743cece9ed52d7f}LIBS}
