#  THIS DIRECTORY
DIRcc1854e4bae72ba2a2fc63cf21e99860:=${ROOT}/algorithm/hough_trans
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIRcc1854e4bae72ba2a2fc63cf21e99860}C:=
${DIRcc1854e4bae72ba2a2fc63cf21e99860}CPP:=hough_trans.cpp 
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIRcc1854e4bae72ba2a2fc63cf21e99860}CFLAGS:=${CFLAGS}
${DIRcc1854e4bae72ba2a2fc63cf21e99860}CXXFLAGS:=${CXXFLAGS}
${DIRcc1854e4bae72ba2a2fc63cf21e99860}INCS:=${INCS}
${DIRcc1854e4bae72ba2a2fc63cf21e99860}LIBS:=${LIBS}

DEP+=${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CPP:%.cpp=${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.d} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}C:%.c=${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.d} 
OBJ+=${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CPP:%.cpp=${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.o} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}C:%.c=${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.o} 
ASM+=${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CPP:%.cpp=${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.s} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}C:%.c=${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.s} 

${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.o: ${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}INCS}
${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.s: ${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}INCS}

${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.o: ${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CXXFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}INCS}
${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.s: ${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}CXXFLAGS} ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}INCS}

# Linking pattern rule for this directory
%.exe: ${DIRcc1854e4bae72ba2a2fc63cf21e99860}/%.o
	${CXX} -o $@ $^ ${${DIRcc1854e4bae72ba2a2fc63cf21e99860}LIBS}
