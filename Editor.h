#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <ncurses.h>

class Editor {

 private: 
  char * file; //the .txt file being edited 
  WINDOW * container;
  WINDOW * editArea;
  int yMax;
  int xMax;
  int currY;
  int currX;
  
 public: 
  Editor(char *); // constructor, takes in a filename as a parameter (could be argv[1]) 
  void handleKeyInput();
  void openMenu(); // when user presses F1
  void displayFile(char *); //display file contents in the edit area  
};

#endif
