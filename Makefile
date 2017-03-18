all: edit 1730ed2

edit.o: edit.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c edit.cpp -lncurses 

edit: edit.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o edit edit.o -lncurses

1730ed2.o: 1730ed2.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c 1730ed2.cpp

1730ed2: 1730ed2.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o 1730ed2 1730ed2.o -lncurses
clean:
	rm -f edit 1730ed2
	rm -f *.o
