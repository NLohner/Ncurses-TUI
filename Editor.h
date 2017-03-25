#ifndef E_H
#define E_H

#include <string>
#include <ncurses.h>

using std::string;

class e
{

 private:
  WINDOW * win; //container 
  FIELD * field [2]; //1st element will be the edit area (text field), 2nd is NULL (last element has to be NULL) 
  FORM * form; //attatch field[0] to form to make it scrollable and gain more functionalities 
  const char * filename;


 public:
  e(const char *); //constructor---takes in a filename (the file to edit)
  void handleKeyInput(); //listen for key input and handle accordingly 
  void openMenu();
  void displayFile(string, int);


  /* Should these be in this class? */
  void save(const char *);
  void saveAs(const char *);
  void exit();
};


#endif
