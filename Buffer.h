#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer{

 public:

  unsigned int MAX_BUFFER_SIZE = 256;

  //string array that holds each line, with each entry in the array
  //being its own line
  String lines[MAX_BUFFER_SIZE];

  //constructor
  Buffer();

  //functions to add, change, and remove lines from the buffer
  void addline(string str, int line);
  void changeline(string str, int line);
  void deleteline(int line);

  //tabs in a string are stored as the '\t' character, so we need
  //a function to turn them into groups of four spaces
  string tabtospace(string str);

};//Buffer

#endif
