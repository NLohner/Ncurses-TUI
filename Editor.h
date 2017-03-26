#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <ncurses.h>
#include <menu.h>

using std::string;

class Editor
{

 private:
  WINDOW * win; //container
  WINDOW * menuWin;
  FIELD * field [2]; //1st element will be the edit area (text field), 2nd is NULL (last element has to be NULL) 
  FORM * form; //attatch field[0] to form to make it scrollable and gain more functionalities
  const char * filename;
  bool is_saved;  

 public:
  Editor(const char *); //constructor---takes in a filename (the file to edit)
  void handleKeyInput(); //listen for key input and handle accordingly 
  void openMenu();
  void displayFile(string, int);
  void clearScreen();
  string promptFileName(); //returns the string that the user typed in (can be converted to c_str() easily)
  int promptYesOrNo(); //returns 1 if user chose YES, 0 if user chose NO 

  string getText(); //return text from screen in string form 
  char * trim_whitespaces(char *); //need for getText

  /* Should these be in this class? */
  void openFile();
  void save(const char *);
  void saveAs(const char *);
  void exit();
};


#endif
