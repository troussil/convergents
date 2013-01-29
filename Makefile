PROG = testPointVector2D

all: $(PROG)

$(PROG): $(PROG).cpp
	g++ -c $< -o $(PROG).o
	g++ $(PROG).o -o $(PROG)

test: 
	./$(PROG)

clean: 
	rm $(PROG) *.o *~
