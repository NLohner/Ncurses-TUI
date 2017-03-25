#ifndef BUFFER_H
#define BUFFER_H

#include <string>

using std::string;

const unsigned int MAX_BUFFER_SIZE = 256;

class Buffer{

  //string array that holds each line. Each index of the array
  //is a new line
  string lines[MAX_BUFFER_SIZE];

 public:

  //constructor
  Buffer();

  //getter
  string getLine(int);

  //functions to add, change, and remove lines from the buffer
  void addLine(string str);
  void changeLine(string str, int line);
  void deleteLine(int);

};//Buffer

//strings passed to the buffer might have the '\t' character,
//so we need to be sure to remove it
string tabToSpace(string);

#endif
