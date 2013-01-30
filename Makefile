PV = PointVector2D
testPV = test$(PV)

all: $(testPV)

$(testPV): $(testPV).cpp $(PV).h
	g++ -c $< -o $(testPV).o
	g++ $(testPV).o -o $(testPV)

test: 
	./$(testPV)

clean: 
	rm $(testPV) *.o *~
