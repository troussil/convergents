PV = PointVector2D
testPV = test$(PV)

testR = testRay


#all cibles
all: $(testPV) $(testR) testConvergents testConvexHull testAlphaShapeStraightLine testAlphaShape

$(testPV): $(testPV).cpp $(PV).h
	g++ -c $< -o $(testPV).o
	g++ $(testPV).o -o $(testPV)

$(testR): $(testR).cpp RayIntersectableStraightLine.h RayIntersectableCircle.h
	g++ -c $< -o $(testR).o
	g++ $(testR).o -o $(testR)

testConvergents: testConvergents.cpp RayIntersectableStraightLine.h
	g++ $< -o $@

testConvexHull: testConvexHull.cpp RayIntersectableCircle.h OutputSensitiveConvexHull.h ConvexHullHelpers.h
	g++ $< -o $@

testAlphaShapeStraightLine: testAlphaShapeStraightLine.cpp RayIntersectableStraightLine.h ConvexHullHelpers.h
	g++ $< -o $@

testAlphaShape: testAlphaShape.cpp RayIntersectableCircle.h OutputSensitiveAlphaShape.h ConvexHullHelpers.h
	g++ $< -o $@

test: 
	./$(testPV) && ./$(testR) && ./testConvergents && ./testConvexHull && ./testAlphaShapeStraightLine && ./testAlphaShape

clean: 
	rm $(testPV) $(testR) testConvergents testConvexHull testAlphaShapeStraightLine testAlphaShape *.o
