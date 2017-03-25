#ifndef BUFFER_H
#define BUFFER_H

#include <string>

using std::string;

const unsigned int MAX_BUFFER_SIZE = 256;

class Buffer{

 public:

  //string array that holds each line, with each entry in the array
  //being its own line
  string lines[MAX_BUFFER_SIZE];

  //constructor
  Buffer();

  //functions to add, change, and remove lines from the buffer
  void addLine(string str);
  void changeLine(string str, int line);
  void deleteLine(int line);

  //tabs in a string are stored as the '\t' character, so we need
  //a function to turn them into groups of four spaces
  string tabToSpace(string str);

};//Buffer

#endif
