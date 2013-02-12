PV = PointVector2D
testPV = test$(PV)

testR = testRay


#all cibles
all: $(testPV) $(testR) testConvergents testConvexHull testAlphaShape

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

testAlphaShape: OutputSensitiveAlphaShape.h testAlphaShape.cpp RayIntersectableCircle.h ConvexHullHelpers.h
	g++ $< -o $@

test: 
	./$(testPV) && ./$(testR) && ./testConvergents && ./testConvexHull && ./testAlphaShape

clean: 
	rm $(testPV) $(testR) testConvergents testConvexHull testAlphaShape *.o
