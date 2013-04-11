CODE := $(shell pwd)

# Adresse des sources, des objets et des includes :
SRCDIR = $(CODE)/src
INCDIR = $(CODE)/inc

# create folders and files
BLDDIR = $(CODE)/build
BINDIR = $(CODE)/build/bin
OBJDIR = $(CODE)/build/obj
DATADIR = $(CODE)/outcome
OBJDIR = $(CODE)/build/obj
TOOLDIR = $(CODE)/tools
DATAFILES = $(DATADIR)/data-`date +'%y.%m.%d-%H:%M'`.txt
DATAWORK = $(DATADIR)/data-means.txt
# Shortcuts
PV = PointVector2D
testPV = test$(PV)
testR = testRay


# Special 
.PHONY: clean

#all cibles
all: |target $(BINDIR)/$(testPV) $(BINDIR)/$(testR) $(BINDIR)/testConvergents $(BINDIR)/testConvexHull $(BINDIR)/testAlphaShapeStraightLine $(BINDIR)/testAlphaShape $(BINDIR)/toolAlphaShape

# Create folders if need
target:
	test -d $(BLDDIR) || mkdir $(BLDDIR)
	test -d $(BINDIR) || mkdir $(BINDIR)
	test -d $(OBJDIR) || mkdir $(OBJDIR)
	test -d $(DATADIR) || mkdir $(DATADIR)
	test -d $(TOOLDIR) || mkdir $(TOOLDIR)
	
# PointVector2D
$(BINDIR)/$(testPV): $(SRCDIR)/$(testPV).cpp $(INCDIR)/$(PV).h
	g++ -c $< -o $(OBJDIR)/$(testPV).o
	g++ $(OBJDIR)/$(testPV).o -o $(BINDIR)/$(testPV)

# Ray
$(BINDIR)/$(testR): $(SRCDIR)/$(testR).cpp $(INCDIR)/RayIntersectableStraightLine.h $(INCDIR)/RayIntersectableCircle.h
	g++ -c $< -o $(OBJDIR)/$(testR).o
	g++ $(OBJDIR)/$(testR).o -o $(BINDIR)/$(testR)

# Convergents
$(BINDIR)/testConvergents: $(SRCDIR)/testConvergents.cpp $(INCDIR)/RayIntersectableStraightLine.h
	g++ $< -o $@

# Convex Hull
$(BINDIR)/testConvexHull: $(SRCDIR)/testConvexHull.cpp $(INCDIR)/RayIntersectableCircle.h $(INCDIR)/OutputSensitiveConvexHull.h $(INCDIR)/ConvexHullHelpers.h
	g++ $< -o $@

# Alpha-shape on straight-line
$(BINDIR)/testAlphaShapeStraightLine: $(SRCDIR)/testAlphaShapeStraightLine.cpp $(INCDIR)/RayIntersectableStraightLine.h $(INCDIR)/ConvexHullHelpers.h
	g++ $< -o $@

# Alpha-shape on circle
$(BINDIR)/testAlphaShape: $(SRCDIR)/testAlphaShape.cpp $(INCDIR)/RayIntersectableCircle.h $(INCDIR)/OutputSensitiveAlphaShape.h $(INCDIR)/ConvexHullHelpers.h
	g++ $< -o $@

# Post Processing alpha-shape on circle
$(BINDIR)/toolAlphaShape: $(SRCDIR)/toolAlphaShape.cpp $(INCDIR)/RayIntersectableCircle.h $(INCDIR)/OutputSensitiveAlphaShape.h $(INCDIR)/OutputSensitiveConvexHull.h $(INCDIR)/ConvexHullHelpers.h
	g++ -std=c++0x $< -o $@
	
# Testing
test: 
	$(BINDIR)/$(testPV) && $(BINDIR)/$(testR) && $(BINDIR)/testConvergents && $(BINDIR)/testConvexHull && $(BINDIR)/testAlphaShapeStraightLine && $(BINDIR)/testAlphaShape

# Post Processing
tool: 
	$(BINDIR)/toolAlphaShape > $(DATAFILES); cp $(DATAFILES) $(DATAWORK); gnuplot -persist $(TOOLDIR)/plop2.gp

# Cleaning
clean: 
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*; rm -f fit.log
