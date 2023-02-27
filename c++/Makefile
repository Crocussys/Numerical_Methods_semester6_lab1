all: main.o
	g++ main.o -o run
	
main.o: main.cpp
	g++ -c main.cpp
	
clean:
	rm -rf *.o

debug: main.o
	g++ -g main.o -o run