#include "Buffer.cpp"
#include "Editor.cpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

Buffer fileToBuffer(Buffer, char*);

Buffer screenToBuffer(Buffer, Editor);

void saveToFile(Buffer, char*);

const unsigned int BUFF_BYTES = 1024;

int main(int argc, char * argv[]){

  Buffer fileBuffer;

  //set the buffer 
  fileBuffer = fileToBuffer(fileBuffer, argv[1]);

  //create editor
  Editor ed(argv[1]);

  //code to pass the Buffer to the Editor and to print it to the screen

  ed.handleKeyInput();

  endwin();
  return EXIT_SUCCESS;

}//main

Buffer fileToBuffer(Buffer buf, char * arg){

  int  fd;
  int off;

  fd = open(arg, O_RDONLY);

  char cBuffer[BUFF_BYTES];

  //reads the whole file into the char array
  while((off = read(fd,cBuffer,BUFF_BYTES)) > 0){}

  int i = 0;

  int line = 0;

  string str = "";

  while(i < BUFF_BYTES && cBuffer[i] != '\0'){

    if(cBuffer[i] == '\n'){

      buf.changeLine(str,line);

      line++;

      str = "";

    }//if

    else{

      str = str + cBuffer[i];

    }//else

    i++;

  }//while

  return buf;

}//openFile

void saveToFile(Buffer buf, char* fileName){

  

}//saveToFile

Buffer screenToBuffer(Buffer buf, Editor ed){

  

}//screenToBuffer
