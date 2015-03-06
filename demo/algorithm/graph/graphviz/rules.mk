#  THIS DIRECTORY
DIR121ba066ae34d22921e7adc4a7dfcc7c:=${ROOT}/algorithm/graph/graphviz
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR121ba066ae34d22921e7adc4a7dfcc7c}C:=
${DIR121ba066ae34d22921e7adc4a7dfcc7c}CPP:=read_write_sample.cpp dot.cpp
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR121ba066ae34d22921e7adc4a7dfcc7c}CFLAGS:=${CFLAGS}
${DIR121ba066ae34d22921e7adc4a7dfcc7c}CXXFLAGS:=${CXXFLAGS}
${DIR121ba066ae34d22921e7adc4a7dfcc7c}INCS:=${INCS} -isystem /usr/local/Cellar/boost/1.57.0/include
${DIR121ba066ae34d22921e7adc4a7dfcc7c}LIBS:=${LIBS} -L/usr/local/Cellar/boost/1.57.0/include -lboost_graph

DEP+=${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CPP:%.cpp=${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.d} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}C:%.c=${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.d} 
OBJ+=${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CPP:%.cpp=${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.o} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}C:%.c=${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.o} 
ASM+=${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CPP:%.cpp=${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.s} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}C:%.c=${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.s} 

${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.o: ${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.c
	${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}INCS}
${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.s: ${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.c
	${CC} -o $@ $< ${ASMFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}INCS}

${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.o: ${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.cpp
	${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CXXFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}INCS}
${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.s: ${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.cpp
	${CXX} -o $@ $< ${ASMFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}CXXFLAGS} ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR121ba066ae34d22921e7adc4a7dfcc7c}/%.o
	${CXX} -o $@ $^ ${${DIR121ba066ae34d22921e7adc4a7dfcc7c}LIBS}
