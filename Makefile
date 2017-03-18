all: 1730ed 1730ed2

1730ed.o: 1730ed.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c 1730ed.cpp

1730ed: 1730ed.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o 1730ed 1730ed.o -lncurses

1730ed2.o: 1730ed2.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c 1730ed2.cpp

1730ed2: 1730ed2.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o 1730ed2 1730ed2.o -lncurses
clean:
	rm -f 1730ed 1730ed2
	rm -f *.o
