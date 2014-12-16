#  THIS DIRECTORY
DIR1fdc67879ea119a25c8dddb2d179b1c0:=${ROOT}/algorithm/heap/fib_heap
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR1fdc67879ea119a25c8dddb2d179b1c0}C:=
${DIR1fdc67879ea119a25c8dddb2d179b1c0}CPP:=test_fib_heap.cpp min_spanning_tree.cpp
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR1fdc67879ea119a25c8dddb2d179b1c0}CFLAGS:=${CFLAGS}
${DIR1fdc67879ea119a25c8dddb2d179b1c0}CXXFLAGS:=${CXXFLAGS}
${DIR1fdc67879ea119a25c8dddb2d179b1c0}INCS:=${INCS}
${DIR1fdc67879ea119a25c8dddb2d179b1c0}LIBS:=${LIBS}

DEP+=${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CPP:%.cpp=${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.d} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}C:%.c=${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.d} 
OBJ+=${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CPP:%.cpp=${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.o} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}C:%.c=${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.o} 
ASM+=${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CPP:%.cpp=${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.s} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}C:%.c=${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.s} 

${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.o: ${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}INCS}
${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.s: ${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}INCS}

${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.o: ${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CXXFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}INCS}
${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.s: ${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}CXXFLAGS} ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR1fdc67879ea119a25c8dddb2d179b1c0}/%.o
	${CXX} -o $@ $^ ${${DIR1fdc67879ea119a25c8dddb2d179b1c0}LIBS}
