#include <ncurses.h>
#include <form.h>
#include "Editor.h"
#include <string>
#include <fcntl.h> //low level I/O
#include <unistd.h> //close()
#include <string.h>

using std::string;

Editor::Editor(const char * filename)
{
  this->filename = filename; 

  initscr();
  noecho();
  
  mvaddstr(1, 0, "F1: Menu");
  mvaddstr(1, COLS/2 - 2, "CSCI 1730 Text Editor!");
  mvaddstr(LINES-2, 0, filename);
  

  /* Create fields (field[0] is the text editing space) */
  field[0] = new_field(LINES-8, COLS-8, 1, 1, 0, 0); //height, width, starty, startx, number of offscreen rows and number of additional working buffers.
  field[1] = NULL; //the last element in array field must be NULL   
  field_opts_off(field[0], O_AUTOSKIP | O_STATIC); //makes the field growable 
  
  /* Create the form */
  form = new_form(field);
  form_opts_off(form, O_NL_OVERLOAD); //gets rid of 'special behavior' when creating a new line using the form driver below 

  /* Create the window (border) */
  win = newwin(LINES-4, COLS-2, 2, 0); //where you want the border
  keypad(win, TRUE);
  wborder(win, '|', '|', '_', '_', ' ',' ', '|', '|');
  
  set_form_win(form, win);
  set_form_sub(form, derwin(win, LINES-6, COLS-6, 1, 1)); //must be smaller/inside win
 
  /* Order is very important here: */
  refresh();
  post_form(form);
  wrefresh(win);
  refresh();
  
  form_driver(form, REQ_INS_CHAR); //so that cursor starts inside the form window
}


void Editor::handleKeyInput() {
  bool listening = true;
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
	form_driver(form, REQ_NEW_LINE);
	break;
      case KEY_BACKSPACE:                                                                                                                                                     
	form_driver(form, REQ_DEL_PREV);
     	break;
      case KEY_F(1):
	openMenu(); //does nothing yet...leave loop
	listening = false;
	break;
      default:
	form_driver(form, ch);
	break;
      } //switch
  }//while	
  
}


void Editor::openMenu() //when user hits F1
{
  
}

void displayError() 
{

}



void Editor::displayFile(string buff, int lineNum) 
{   
  form_driver(form, REQ_BEG_FIELD);

  for(int i = 0; i<lineNum; i++) 
    form_driver(form, REQ_DOWN_CHAR);
  
  waddstr(win, buff.c_str()); //may not work because of waddstr...test first, then I have another idea 
}


	
void Editor::save(const char * filename)
{

}

void Editor::saveAs(const char * filename)
{

}

void Editor::exit() 
{
  unpost_form(form);
  free_form(form);
  free_field(field[0]);
  endwin();
  //close(fd); 
}



int main(int argc, const char * argv[]) 
{
  Editor ed = Editor("newfile.txt"); //create the editor 
  ed.handleKeyInput();
  ed.exit();
  return 0;

}
