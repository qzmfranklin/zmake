################################################################################
#  A set of template files to build projects with a single Makefile
#  Copyright (C) 2013-2014 Zhongming Qu <qzmfrank@umich.edu>
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
################################################################################
#  Sample Makefile for out-of-source build
################################################################################
#	ROOT LEVEL INFO - MAKEFILE SPECIFIC
OUT_OF_SOURCE:=TRUE# TRUE=out-of-source build, otherwise in-source build
BUILD:=.# out-of-source build directory, unused if in-source build
#QUIET:=@# uncomment to silence
ROOT :=~/codes/zmake/demo/src# root of source files
-include ${ROOT}/root.mk
-include ${ROOT}/heapsort/branch.mk
-include ${ROOT}/utils/branch.mk
-include ${ROOT}/diffpar/branch.mk
-include ${DEP}# INCLUDE DEPENDENCY FILES
################################################################################ 
#	LIST LINKING RULES
diffpar.exe: diffpar.o utils.o
test_heapsort.exe: heapsort.o test_heapsort.o utils.o
