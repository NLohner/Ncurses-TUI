#include <string>
#include <ncurses.h>
#include "Editor.h"

Editor::Editor(char * fileName) { 

  initscr();
  noecho();
  getmaxyx(stdscr, yMax, xMax);
  
  container = newwin(yMax - 4, xMax-2, 1, 0); // used for visible border
  editArea = newwin(yMax - 6, xMax-6, 2, 2); // limits in which user can write text (invisible border)
  
  wborder(container, '|', '|', '_', '_', ' ',' ', '|', '|'); 
  keypad(editArea, TRUE);
  scrollok(editArea, TRUE);

  mvwprintw(stdscr, 0, 0, "F1: Menu");
  mvwprintw(stdscr, 0, xMax/2 - 10, "CSCI 1730 Editor!");
  mvwprintw(stdscr, yMax-3, 0, fileName); // print .txt name at bottom left 
  
  wmove(editArea, 0, 0);
  currY = 0;
  currX = 0;

  refresh();
  wrefresh(container);
  wrefresh(editArea);
}

void Editor::displayFile(char * file) {

  //print contents of file into editArea

}

void Editor::handleKeyInput() {
  int key;
  
  int maxEdX;
  int maxEdY;
  getmaxyx(editArea, maxEdY, maxEdX);

  while(1) {
    wmove(editArea, currY, currX);
    key = wgetch(editArea); //important: wgetch, not just getch()

    switch(key) {
      
      /* CASES */
    case KEY_UP: //remember to handle scrolling
      if(currY > 0)
	currY--;
      break;

    case KEY_DOWN:
      if(currY < maxEdY - 1)
	currY++;
      break;

    case KEY_LEFT:
      if(currX > 0)
	currX--;
      break;

    case KEY_RIGHT:
      if(currX < maxEdX - 1)
	currX++;
      break;

    case 127: //BACKSPACE
      if(currX > 0)   
	mvwdelch(editArea, currY, --currX);
      break;
      
    case 10: //ENTER
      waddch(editArea, '\n');
      currY++;
      currX = 0;
      break;

    default:
      if(currX < maxEdX) {
	waddch(editArea, key);
	currX++;
      }
      
      else { //at end of line
	if(currY < maxEdY) { 
	  mvwaddch(editArea, ++currY, currX = 0, key);
      }
      break;
    }

    wrefresh(editArea);
  }//while
    
  }
  

}



int main(int argc, char * argv []) { //test
  
  Editor ed1{argv[1]};
  ed1.handleKeyInput();
  
  endwin();
  return 0;
  
}
