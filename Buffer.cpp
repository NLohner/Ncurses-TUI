#include "Buffer.h"

Buffer::Buffer(){
}//Buffer

string Buffer::getLine(int line){

  return lines[line];

}//getLine

void Buffer::addLine(string str){

  str = tabToSpace(str);

  bool empty = true;

  int pos = 0;

  while(empty && pos < MAX_BUFFER_SIZE){

    empty = !lines[pos].empty();

    if(empty) pos++;

  }//while

  if(lines[pos].empty()) lines[pos] = str;

}//addLine

void Buffer::changeLine(string str, int line){

  str = tabToSpace(str);

  lines[line] = str;

}//addLine

void Buffer::deleteLine(int line){

  lines[line] = "";

}//deleteLine

string tabToSpace(string str){

  int pos;

  pos = str.find("\t");

  if(pos == string::npos){

    return str;

  }//if

  else{

    return tabToSpace(str.replace(pos,1,"    "));

  }//else

}//tabToSpace
