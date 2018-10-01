sum: sum.o 
	g++ -g -o sum sum.cpp

debug: sum.o
	g++ -DDEBUG -o sum sum.cpp