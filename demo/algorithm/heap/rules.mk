#  THIS DIRECTORY
DIRf4a4570f3fd5b5eb439de4ea710b424e:=${ROOT}/algorithm/heap
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRf4a4570f3fd5b5eb439de4ea710b424e}C:=
${DIRf4a4570f3fd5b5eb439de4ea710b424e}CPP:=heap.cpp test_heap.cpp
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRf4a4570f3fd5b5eb439de4ea710b424e}CFLAGS:=${CFLAGS}
${DIRf4a4570f3fd5b5eb439de4ea710b424e}CXXFLAGS:=${CXXFLAGS}
${DIRf4a4570f3fd5b5eb439de4ea710b424e}INCS:=${INCS}
${DIRf4a4570f3fd5b5eb439de4ea710b424e}LIBS:=${LIBS}

DEP+=${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CPP:%.cpp=${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.d} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}C:%.c=${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.d} 
OBJ+=${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CPP:%.cpp=${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.o} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}C:%.c=${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.o} 
ASM+=${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CPP:%.cpp=${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.s} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}C:%.c=${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.s} 

${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.o: ${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}INCS}
${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.s: ${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}INCS}

${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.o: ${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CXXFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}INCS}
${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.s: ${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}CXXFLAGS} ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRf4a4570f3fd5b5eb439de4ea710b424e}/%.o
	${CXX} -o $@ $^ ${${DIRf4a4570f3fd5b5eb439de4ea710b424e}LIBS}
