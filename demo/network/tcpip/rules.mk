#  THIS DIRECTORY
DIR448aeacedd83e5a6da8c9effcfbf475b:=${ROOT}/network/tcpip
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR448aeacedd83e5a6da8c9effcfbf475b}C:=
${DIR448aeacedd83e5a6da8c9effcfbf475b}CPP:=inet_utils.cpp ipmsg.cpp showip.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR448aeacedd83e5a6da8c9effcfbf475b}CFLAGS:=${CFLAGS}
${DIR448aeacedd83e5a6da8c9effcfbf475b}CXXFLAGS:=${CXXFLAGS}
${DIR448aeacedd83e5a6da8c9effcfbf475b}INCS:=${INCS}
${DIR448aeacedd83e5a6da8c9effcfbf475b}LIBS:=${LIBS}

DEP+=${${DIR448aeacedd83e5a6da8c9effcfbf475b}CPP:%.cpp=${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.d} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}C:%.c=${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.d} 
OBJ+=${${DIR448aeacedd83e5a6da8c9effcfbf475b}CPP:%.cpp=${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.o} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}C:%.c=${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.o} 
ASM+=${${DIR448aeacedd83e5a6da8c9effcfbf475b}CPP:%.cpp=${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.s} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}C:%.c=${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.s} 

${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.o: ${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}CFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}INCS}
${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.s: ${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}CFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}INCS}

${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.o: ${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}CXXFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}INCS}
${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.s: ${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}CXXFLAGS} ${${DIR448aeacedd83e5a6da8c9effcfbf475b}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR448aeacedd83e5a6da8c9effcfbf475b}/%.o
	${CXX} -o $@ $^ ${${DIR448aeacedd83e5a6da8c9effcfbf475b}LIBS}
