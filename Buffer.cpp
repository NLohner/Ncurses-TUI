#include "Buffer.h"

Buffer::Buffer(){

  for(int i = 0; i < MAX_BUFFER_SIZE; i++){

    lines[i] = '\b';

  }//for

}//Buffer

string Buffer::getLine(int line){

  return lines[line];

}//getLine

void Buffer::addLine(string str){

  str = tabToSpace(str);

  bool empty = true;

  int pos = 0;

  while(empty && pos < MAX_BUFFER_SIZE){

    empty = !((lines[pos])[0] == '\b');

    if(empty) pos++;

  }//while

  if((lines[pos])[0] == '\b') lines[pos] = str;

}//addLine

void Buffer::changeLine(string str, int line){

  str = tabToSpace(str);

  lines[line] = str;

}//addLine

void Buffer::deleteLine(int line){

  lines[line] = '\0';

}//deleteLine

string tabToSpace(string str){

  int pos;

  pos = str.find("\t");

  if(pos == string::npos){

    return str;

  }//if

  else{

    return tabToSpace(str.replace(pos,1,"   "));

  }//else

}//tabToSpace
