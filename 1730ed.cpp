#include "Buffer.cpp"
#include "Editor.cpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char * argv[]){

  Buffer buff;
  int fdes;
  
  fdes = open(argv[1], O_RDONLY);
  
  if(fdes == -1 || argc != 2 ) {
    Editor ed("New File");
    ed.handleKeyInput();
  }
  
  else { //if argv[1] is an existing file that can be opened 
    Editor ed(argv[1]);
    buff = ed.fileToBuffer(buff, argv[1]);  
    ed.bufferToScreen(buff, ed); //put file's contents into edit area 
    ed.handleKeyInput();
  }
  
  
  return EXIT_SUCCESS;
  
}//main

