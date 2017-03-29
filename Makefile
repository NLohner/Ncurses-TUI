all: 1730ed Buffer.o Editor.o

FLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors

Buffer.o: Buffer.cpp
	g++ $(FLAGS) -c Buffer.cpp

Editor.o: Editor.cpp
	g++ $(FLAGS) -c Editor.cpp

1730ed: 1730ed.o
	g++ $(FLAGS) -o 1730ed 1730ed.cpp -lncurses -lform

1730ed.o: 1730ed.cpp
	g++ $(FLAGS) -c 1730ed.cpp

clean:
	rm -f edit 1730ed2 1730ed
	rm -f *.o *~ \#* *.gch