all: 1730ed 1730ed2 Buffer.o 1730ed

FLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors

edit.o: edit.cpp
	g++ $(FLAGS) -c edit.cpp -lncurses 

edit: edit.o
	g++ $(FLAGS) -o edit edit.o -lncurses

1730ed2.o: 1730ed2.cpp
	g++ $(FLAGS) -c 1730ed2.cpp

1730ed2: 1730ed2.o
	g++ $(FLAGS) -o 1730ed2 1730ed2.o -lncurses

Buffer.o: Buffer.cpp
	g++ $(FLAGS) -c Buffer.cpp

1730ed: 1730ed.o
	g++ $(FLAGS) -o 1730ed 1730ed.cpp -lncurses

1730ed.o: 1730ed.cpp
	g++ $(FLAGS) -c 1730ed.cpp

clean:
	rm -f edit 1730ed2 cs1730ed
	rm -f *.o
