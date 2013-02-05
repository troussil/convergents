PV = PointVector2D
testPV = test$(PV)

testR = testRay


#all cibles
all: $(testPV) $(testR) testConvergents

$(testPV): $(testPV).cpp $(PV).h
	g++ -c $< -o $(testPV).o
	g++ $(testPV).o -o $(testPV)

$(testR): $(testR).cpp RayIntersectableStraightLine.h RayIntersectableCircle.h
	g++ -c $< -o $(testR).o
	g++ $(testR).o -o $(testR)

testConvergents: testConvergents.cpp
	g++ $< -o $@

test: 
	./$(testPV) && ./$(testR) && ./testConvergents

clean: 
	rm $(testPV) $(testR) testConvergents *.o *~
