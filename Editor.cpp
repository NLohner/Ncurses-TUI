#include <ncurses.h>
#include <form.h>
#include "Editor.h"
#include <string>
#include <fcntl.h> //low level I/O
#include <unistd.h> //close()
#include <string.h>
#include <vector>

using std::string;
using std::vector;
const unsigned int BUFF_BYTES = 1024;

Editor::Editor(const char * filename)
{
  this->filename = filename; 
  is_saved = false;

  initscr();
  noecho();
  
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  mvaddstr(1, 0, "F1: Menu");
  mvaddstr(1, COLS/2 - 8, "CSCI 1730 Text Editor!");
  mvaddstr(LINES-2, 0, filename);
  attroff(COLOR_PAIR(1));  

  /* Create fields (field[0] is the text editing space) */
  field[0] = new_field(LINES-7, COLS-7, 1, 1, 0, 0); //height, width, starty, startx, number of offscreen rows and number of additional working buffers.
  field[1] = NULL; //the last element in array field must be NULL   
  field_opts_off(field[0], O_AUTOSKIP | O_STATIC); //makes the field growable 
  
  /* Create the form */
  form = new_form(field);
  form_opts_off(form, O_NL_OVERLOAD); //gets rid of 'special behavior' when creating a new line using the form driver below 

  /* Create the window (border) */
  win = newwin(LINES-4, COLS-2, 2, 0); //where you want the border
  keypad(win, TRUE);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  wattron(win, COLOR_PAIR(3));
  wborder(win, '|', '|', '_', '_', ' ',' ', '|', '|');
  wattroff(win, COLOR_PAIR(3));
  set_form_win(form, win);
  set_form_sub(form, derwin(win, LINES-6, COLS-6, 1, 1)); //must be smaller/inside win


  /* Order is very important here: */
  refresh();
  post_form(form);
  wrefresh(win);
  refresh();

  form_driver(form, REQ_INS_CHAR); //so that cursor starts inside the form window
  
}


/*
 * In order for these keys to work, you must go to PuTTY keyboard settings
 * and do two things: 1) Make sure Xterm R6 is selected, and 
 * 2) Ensure that Control-?(127) is selected for the Backspace key.
 */
void Editor::handleKeyInput() {
  bool listening = true;
  Buffer buf;
  while(listening) {
    int ch = wgetch(win); //notice: keypad is on WINDOW
   
    switch(ch)
      {
      case KEY_DOWN:
	form_driver(form, REQ_DOWN_CHAR);
	break;
      case KEY_UP:
	form_driver(form, REQ_UP_CHAR);
	break;
      case KEY_RIGHT:
	form_driver(form, REQ_NEXT_CHAR);
	break;
      case KEY_LEFT:
	form_driver(form, REQ_PREV_CHAR);
	break;
      case 10: //ENTER 
	is_saved = false; //because text was changed/edited 
	form_driver(form, REQ_NEW_LINE);
	break;
      case KEY_BACKSPACE:
	is_saved = false;
	form_driver(form, REQ_DEL_PREV);
     	break;
      case KEY_F(1): //the menu is the only way to exit other than CTRL-x
	listening = false; 
	openMenu();
	break;
      default:
	is_saved = false;
	form_driver(form, ch);
	break;
      } //switch
  }//while	
 
}


void Editor::openMenu() //when user hits F1
{
  init_pair(2, COLOR_WHITE, COLOR_BLUE);
  menuWin = newwin(6, COLS-12, 5, 5); //lines, cols, starty, startx
  wbkgd(menuWin, COLOR_PAIR(2)); 
  //wborder(menuWin, '|', '|', '_', '_', ' ',' ', '|', '|');
  refresh(); //may have to refresh win
  wrefresh(menuWin);
  keypad(menuWin, true);

  const char * choices[4] = {"Open", "Save", "Save As", "Exit"};
  int toggle = 0; //index of choice that's supposed to be highlighted

  while(1)
    {
      for(int i = 0; i < 4; i++) {
	if(i == toggle)
	  wattron(menuWin, A_REVERSE); //highlight at toggle
	mvwprintw(menuWin, i+1, 1, choices[i]); //print choices
	wattroff(menuWin, A_REVERSE); //toggle off anything that was highlighted before 
      } //for
      
      int ch = wgetch(menuWin); //get char from not just any win, but menuWin
      switch(ch)
	{
	case KEY_UP:
	  toggle--;
	  if(toggle == -1)
	    toggle = 0; //don't let it go past bounds
	  break;
	case KEY_DOWN:
	  toggle++;
	  if(toggle == 4)
	    toggle = 3;
	  break;

	  /* Don't put case 10 (ENTER) inside of here. Put outside so that "break" is for the WHILE loop */

	case 10: //ENTER
	  break;
	}
      
      /* User presses Enter on a choice */
      if(ch == 10) { 
	
	if(toggle == 0) openFile();
	if(toggle == 1) save(filename);
	if(toggle == 2) saveAs(filename);
	if(toggle == 3) {
	  promptSaveChanges();
	  exit();
	} 
	break; //leave while loop
	
      } //if
    } //while
} //openMenu()

void Editor::displayError()
{
  werase(menuWin);
  mvwprintw(menuWin, 1, 1, strerror(errno));
  if(errno == EACCES)
    mvwprintw(menuWin, 2, 1, "Please only try opening files that you have access to. Returning to Menu.");
  if(errno == ENOENT)
    mvwprintw(menuWin, 2, 1, "Please try opening a file that exists.");
  refresh();
  wrefresh(menuWin);
  
} 

string Editor::promptFileName() /* For the Menu options */
{
  int letter;
  vector<char> vec = {}; //to keep track of what user types

  werase(menuWin); //gets rid of border!
  mvwprintw(menuWin, 1, 1, "Enter name of file (note: backspaces not allowed):");
  wmove(menuWin, 2, 1);
  while(1) {
    letter = wgetch(menuWin);
    if(letter == '\n')
      break;
    else {
      waddch(menuWin, letter);
      vec.push_back(letter);
    }
  }
  string nameEntered = string(vec.begin(), vec.end()); //make a string out of the vector<char>
  return nameEntered; 

}

/*
 * Opens a menu with choices "Yes" and "No" for the user to choose from.
 * @return 1 if user chose YES, 0 if user chose NO. 
 */
int Editor::promptYesOrNo() 
{
  const char * choices[2] = {"Yes" , "No"};
  int toggle = 0;
 
  while(1) 
    { 
      for(int i = 0; i < 2; i++) {
	if(i == toggle)
	  wattron(menuWin, A_REVERSE); //highlight at toggle
	wattron(menuWin, COLOR_PAIR(2));
	mvwprintw(menuWin, i+2, 1, choices[i]); //print choices
	wattroff(menuWin, COLOR_PAIR(2));
	wattroff(menuWin, A_REVERSE); //toggle off anything that was highlighted before
      } //for
      
      
      int ch = wgetch(menuWin); //get char from not just any win, but menuWin
      switch(ch)
	{
	case KEY_UP:
	  toggle--;
	  if(toggle == -1)
	    toggle = 0; //don't let it go past bounds
	  break;
	case KEY_DOWN:
	  toggle++;
	  if(toggle == 2)
	    toggle = 1;
	  break;
	  
	case 10: //ENTER
	  break;
	}
      if(ch == 10) {
	if(toggle == 0) return 1; //return 1 for YES 
	if(toggle == 1) return 0; //return 0 for NO
      } //if
    }//while
}//promptYesOrNo()


void Editor::promptSaveChanges() {
  if(is_saved == false) {
    refresh();
    wrefresh(menuWin);
    werase(menuWin);
    mvwprintw(menuWin, 1, 1, "Save changes to current file before leaving?");
    int yesOrNo = promptYesOrNo();
  if(yesOrNo == 1) //YES
    {
      //code to save 
    }
  else if(yesOrNo == 0) //NO
    {
      //don't save
    }
  }

}

void Editor::displayFile(string buff, int lineNum)
{
  form_driver(form, REQ_BEG_FIELD); //go to very beginning of edit area

  for(int i = 0; i<lineNum; i++)
    form_driver(form, REQ_DOWN_CHAR); //go down lineNum number of lines
  
  const char * arr = buff.c_str();  //turn string that was passed in into a c_string
  for(unsigned int i = 0; i < strlen(arr); i++)
    form_driver(form, arr[i]); //add each char from the string individually
}

/**
 * Moves contents (chars) of file to a buffer.
 * @param buf the Buffer to store file contents in
 * @param arg the name of the file to get contents from
 * @returns the buffer on success, NULL on error 
 */
Buffer Editor::fileToBuffer(Buffer buf, const char * arg){
  
  int fd;
  int off;

  fd = open(arg, O_RDONLY);
  
  char cBuffer[BUFF_BYTES];

  //reads the whole file into the char array
  while((off = read(fd,cBuffer,BUFF_BYTES)) > 0){}
  unsigned int i = 0;
  int line = 0;
  string str = "";
  while(i < BUFF_BYTES && cBuffer[i] != '\0'){
    if(cBuffer[i] == '\n'){
      buf.changeLine(str,line);
      line++;
      str = "";
    }//if
    
    else{
      str += cBuffer[i];
    }//else
    i++;
  }//while
  
  return buf;
} //fileToBuffer()

/*
void saveToFile(Buffer buf, char* fileName){

  

}//saveToFile


Buffer Editor::screenToBuffer(Buffer buf, Editor ed){

  

} //screenToBuffer()
*/

void Editor::bufferToScreen(Buffer buf, Editor ed){

  int i = 0;

  while((buf.getLine(i))[0] != '\b'){

    ed.displayFile(buf.getLine(i), i);

    i++;

  } //while

}//bufferToScreen()



void Editor::openFile() 
{
  int fd;
  string name = promptFileName();
 
  /* Try to open specified filename */
  if((fd = open(name.c_str(), O_RDONLY)) == -1) {
    displayError();
    getch(); //wait
    openMenu();
  }
  
  else { //if open was successful
    Buffer fileBuffer;
    fileBuffer = fileToBuffer(fileBuffer, name.c_str()); //uses open() and read()
    promptSaveChanges();  
    exit();

    /* Create new Editor for 'name' */
    Editor neditor(name.c_str());
    bufferToScreen(fileBuffer, neditor); //dump file contents into Editor 
    neditor.handleKeyInput();    
  }
}
 



void Editor::clearScreen()
{
  form_driver(form, REQ_CLR_FIELD);
}
	
void Editor::save(const char * filename)
{
  int fd;
  string s = "(proof of save): This file has been saved! ";
  fd = open(filename, O_WRONLY);
  write(fd, s.c_str(), strlen(s.c_str())); //check errors here

  //code to save

  werase(menuWin);
  mvwprintw(menuWin, 1, 1, "Save successful. Press any KEY to return to Menu.");
  refresh();
  wrefresh(menuWin);
  getch();
  is_saved = true;
  openMenu();
	      
}
  

void Editor::saveAs(const char * filename)
{
  int filedes;
  string newname = promptFileName();
  
  if((filedes = open(newname.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_TRUNC)) == -1) { //crate new file  
    if(errno == EEXIST) {
      wclear(menuWin);
      mvwprintw(menuWin, 1, 1, strerror(errno));
      mvwprintw(menuWin, 1, 12, ". Override existing file?");
   
      /* YES OR NO */
      int yon = promptYesOrNo(); //override or no
      if(yon == 1) { //YES 
	werase(menuWin);
	mvwprintw(menuWin, 1, 1, "Save successful. Press any KEY to return to Menu.");
	refresh();
	wrefresh(menuWin);
	getch();
	is_saved = true;
	openMenu();
      }
      else if(yon == 0) { //NO
	werase(menuWin);
	mvwprintw(menuWin, 1, 1, "Did not override. Save again with a different name.");
	refresh();
	wrefresh(menuWin);
	getch();
	openMenu();
      }
      
    }
    else { //error other than EEXIST 
      displayError();
      openMenu();
    }
    refresh();
    wrefresh(menuWin);

  }
  else{ //no error returned 
    //code to saveas behind the scenes
    string s = "This file has been saved.";
    write(filedes, s.c_str(), strlen(s.c_str())); 
    
    werase(menuWin);
    mvwprintw(menuWin, 1, 1, "Save successful. Press any KEY to return to Menu.");
    refresh();
    wrefresh(menuWin);
    getch();
    is_saved = true;
    openMenu();
    } 
}

void Editor::exit() 
{    
  unpost_form(form);
  free_form(form);
  free_field(field[0]);
  erase();
  //endwin(); 
  //close(fd); 
}

