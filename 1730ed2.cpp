#include <ncurses.h>

int main()
{
  initscr(); 
  noecho();
 

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax); //size of stdscr

  WINDOW * outer = newwin(yMax-5, xMax-2, 2, 0); //used for border
  WINDOW * edspace = newwin(yMax-7, xMax-6, 3, 2); //used as limits in which user can write text (invisible border) 
  wborder(outer, '|', '|', '_', '_', ' ',' ', '|', '|'); //left vertical, right vertical, top horizontal, bottom horizontal, 4 corners
  keypad(edspace, true);

  refresh();
  wrefresh(outer);
  wrefresh(edspace);

  int key;

  wmove(edspace, 0,0);
  int currY = 0;
  int currX = 0;

  while(1) {
    
    key = getch();
    switch(key) {
     
    case 127: //BACKSPACE
      waddch(edspace, 'X');
      break;
    
    case 8:
      waddch(edspace, 'X');
      break;
      
    case KEY_UP:
      wmove(edspace, ++currY, currX);
      break;
    case KEY_DOWN:
      wmove(edspace, --currY, currX);
      break;
    case KEY_RIGHT:
      
      break;
    case KEY_LEFT:
      wmove(edspace, currY, --currX);
      break;
    default:
      waddch(edspace, key);
      break;
    
    

    }
    wrefresh(edspace); //if this were outside the while loop, then it wouldn't refresh til completely done editing



  }//while
  
  endwin();
  return 0;
}
