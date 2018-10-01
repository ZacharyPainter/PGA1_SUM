sum: sum.o 
	g++ -g -o sum sum.o

sum.o: sum.cpp
	g++ -g -c sum.cpp