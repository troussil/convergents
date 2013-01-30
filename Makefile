PV = PointVector2D
testPV = test$(PV)

R = RayIntersectableStraightLine
testR = testRay


#all cibles
all: $(testPV) $(testR)

$(testPV): $(testPV).cpp $(PV).h
	g++ -c $< -o $(testPV).o
	g++ $(testPV).o -o $(testPV)

$(testR): $(testR).cpp $(R).h
	g++ -c $< -o $(testR).o
	g++ $(testR).o -o $(testR)

test: 
	./$(testPV) && ./$(testR)

clean: 
	rm $(testPV) $(testR) *.o *~
