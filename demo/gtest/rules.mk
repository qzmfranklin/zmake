# This is a hand crafted file to integrate the Google Test Framework into this
# Makefile system. This is significantly different from the rules.mk generated
# by the zmake script. Please do not change this file unless you really know
# what you are doing.
#
# gtest source files ($(DIR_GTEST)/src/*.cc) are built in-place. gtest samples
# and its static libraries are left $(DIR_GTEST) to be easy to use by other
# modules.
#
# Do not use $(DIR_GTEST) anywhere outside of this file

DIR_GTEST:=$(ROOT)/gtest
$(DIR_GTEST)CC:=$(wildcard $(DIR_GTEST)/src/*.cc) $(wildcard $(DIR_GTEST)/samples/*.cc)

$(DIR_GTEST)CFLAGS:=$(CFLAGS)
$(DIR_GTEST)CXXFLAGS:=$(CXXFLAGS)
$(DIR_GTEST)INCS:=$(INCS)
$(DIR_GTEST)LIBS:=$(LIBS)

DEP+=${$(DIR_GTEST)CC:%.cc=%.d}
#OBJ+=${$(DIR_GTEST)CC:%.cc=%.o}
ASM+=${$(DIR_GTEST)CC:%.cc=%.s}

$(DIR_GTEST)/%.o: $(DIR_GTEST)/%.cc
	$(QUIET)$(CXX) -o $@ -c $< $(DEPFLAGS) ${$(DIR_GTEST)CXXFLAGS} ${$(DIR_GTEST)INCS}
	$(QUIET)echo "Compiling $(GREEN)$(notdir $<) $(NONE)..."
$(DIR_GTEST)/%.s: $(DIR_GTEST)/%.cc
	$(QUIET)$(CXX) -o $@ $< $(ASMFLAGS) ${$(DIR_GTEST)CXXFLAGS} ${$(DIR_GTEST)INCS}
	$(QUIET)echo "Assembly listing $(CYAN)$(notdir $<) $(NONE)..."

%.exe: $(DIR_GTEST)/samples/%.o
	$(QUIET)$(CXX) -o $@ $^ ${$(DIR_GTEST)LIBS}
	$(QUIET)echo "Linking $(MAGENTA)$(notdir $@) $(NONE)..."

$(DIR_GTEST)/gtest.a : $(DIR_GTEST)/src/gtest-all.o
	$(QUIET)$(AR) $(ARFLAGS) $@ $^
	$(QUIET)echo "Linking $(MAGENTA)$(notdir $@) $(NONE)..."

$(DIR_GTEST)/gtest_main.a : $(DIR_GTEST)/src/gtest-all.o $(DIR_GTEST)/src/gtest_main.o
	$(QUIET)$(AR) $(ARFLAGS) $@ $^
	$(QUIET)echo "Linking $(MAGENTA)$(notdir $@) $(NONE)..."
