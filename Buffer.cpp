#include "Buffer.h"

Buffer::Buffer(){
}//Buffer

void Buffer::addLine(string str){

  for(int i = 0; i < MAX_BUFFER_SIZE; i++){

    if(lines[i].empty()) lines[i] = str;

  }//for

}//addLine

void Buffer::changeLine(string str, int line){

  lines[line] = str;

}//addLine

void Buffer::deleteLine(int line){

  lines[line] = line;

}//deleteLine

string Buffer::tabToSpace(string str){

  int pos;

  while((pos = str.find('\t')) != string::npos){

    str.replace(pos,1,"    ");

  }//while

}//tabToSpace

