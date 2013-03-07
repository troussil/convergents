CODE := $(shell pwd)

# Adresse des sources, des objets et des includes :
SRCDIR = $(CODE)/src
INCDIR = $(CODE)/lib

# create bin and obj folders
BLDDIR = $(CODE)/build
BINDIR = $(CODE)/build/bin
OBJDIR = $(CODE)/build/obj
 
# Shortcuts
PV = PointVector2D
testPV = test$(PV)
testR = testRay


# Special 
.PHONY: clean

#all cibles
all: |target $(BINDIR)/$(testPV) $(BINDIR)/$(testR) $(BINDIR)/testConvergents $(BINDIR)/testConvexHull $(BINDIR)/testAlphaShapeStraightLine $(BINDIR)/testAlphaShape

# Create folders if need
target:
	test -d $(BLDDIR) || mkdir $(BLDDIR)
	test -d $(BINDIR) || mkdir $(BINDIR)
	test -d $(OBJDIR) || mkdir $(OBJDIR)

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

# Testing
test: 
	$(BINDIR)/$(testPV) && $(BINDIR)/$(testR) && $(BINDIR)/testConvergents && $(BINDIR)/testConvexHull && $(BINDIR)/testAlphaShapeStraightLine && $(BINDIR)/testAlphaShape

# Cleaning
clean: 
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*
