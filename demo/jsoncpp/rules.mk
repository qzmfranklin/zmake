#  THIS DIRECTORY
TMP:=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
$(TMP)DIR:=$(TMP)

#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
$($(TMP)DIR)C:=$(wildcard *.c)
$($(TMP)DIR)CC:=$(wildcard *.cc)
$($(TMP)DIR)CPP:=$(wildcard *.cpp)
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
$($(TMP)DIR)CFLAGS:=$(CFLAGS)
$($(TMP)DIR)CXXFLAGS:=$(CXXFLAGS)
$($(TMP)DIR)INCS:=$(INCS)
$($(TMP)DIR)LIBS:=$(LIBS)

DEP:=$(DEP) $($($(TMP)DIR)C:%.c=$($(TMP)DIR)/%.d) $($($(TMP)DIR)CC:%.cc=$($(TMP)DIR)/%.d) $($($(TMP)DIR)CPP:%.cpp=$($(TMP)DIR)/%.d)
OBJ:=$(OBJ) $($($(TMP)DIR)C:%.c=$($(TMP)DIR)/%.o) $($($(TMP)DIR)CC:%.cc=$($(TMP)DIR)/%.o) $($($(TMP)DIR)CPP:%.cpp=$($(TMP)DIR)/%.o)
ASM:=$(ASM) $($($(TMP)DIR)C:%.c=$($(TMP)DIR)/%.s) $($($(TMP)DIR)CC:%.cc=$($(TMP)DIR)/%.s) $($($(TMP)DIR)CPP:%.cpp=$($(TMP)DIR)/%.s)

$($(TMP)DIR)/%.o: $($(TMP)DIR)/%.c
	$(QUIET)$(CC) -o $@ -c $< $(DEPFLAGS) $($($(TMP)DIR)CFLAGS) $($($(TMP)DIR)INCS)
	$(QUIET)echo "Compiling $(GREEN)$(notdir $<) $(NONE)..."
$($(TMP)DIR)/%.s: $($(TMP)DIR)/%.c
	$(QUIET)$(CC) -o $@ $< $(ASMFLAGS) $($($(TMP)DIR)CFLAGS) $($($(TMP)DIR)INCS)
	$(QUIET)echo "Assembly listing $(CYAN)$(notdir $<) $(NONE)..."

$($(TMP)DIR)/%.o: $($(TMP)DIR)/%.cc
	$(QUIET)$(CXX) -o $@ -c $< $(DEPFLAGS) ${$($(TMP)DIR)CXXFLAGS} ${$($(TMP)DIR)INCS}
	$(QUIET)echo "Compiling $(GREEN)$(notdir $<) $(NONE)..."
$($(TMP)DIR)/%.s: $($(TMP)DIR)/%.cc
	$(QUIET)$(CXX) -o $@ $< $(ASMFLAGS) ${$($(TMP)DIR)CXXFLAGS} ${$($(TMP)DIR)INCS}
	$(QUIET)echo "Assembly listing $(CYAN)$(notdir $<) $(NONE)..."

$($(TMP)DIR)/%.o: $($(TMP)DIR)/%.cpp
	$(QUIET)$(CXX) -o $@ -c $< $(DEPFLAGS) $($($(TMP)DIR)CXXFLAGS) $($($(TMP)DIR)INCS)
	$(QUIET)echo "Compiling $(GREEN)$(notdir $<) $(NONE)..."
$($(TMP)DIR)/%.s: $($(TMP)DIR)/%.cpp
	$(QUIET)$(CXX) -o $@ $< $(ASMFLAGS) $($($(TMP)DIR)CXXFLAGS) $($($(TMP)DIR)INCS)
	$(QUIET)echo "Assembly listing $(CYAN)$(notdir $<) $(NONE)..."

# Linking pattern rule for this directory
%.exe: $($(TMP)DIR)/%.o
	$(QUIET)$(CXX) -o $@ $^ $($($(TMP)DIR)LIBS)
	$(QUIET)echo "Linking $(MAGENTA)$(notdir $@) $(NONE)..."

# Recursive inclusion
-include $(wildcard $(TMP)/*/$(notdir $(lastword $(MAKEFILE_LIST))))
