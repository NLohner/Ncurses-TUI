#include <ncurses.h> 
#include <string>

using std::string;

int main(const int argc, char * argv []) {
  
  initscr();
  noecho();
  curs_set(0); //make cursor invisible

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax); //size of stdscr

  WINDOW * win = newwin(6, xMax-20, yMax-(yMax/2), 4); //(nlines, ncols, begin_y, begin_x). Based off xMax and yMax so that it scales with screen size
  wborder(win, '|', '|', '_', '_', ' ',' ', '|', '|'); //left vertical, right vertical, top horizontal, bottom horizontal, 4 corners
  keypad(win, true); //so the user can go up/down the menu using keys
  
  refresh();
  wrefresh(win);
  
  string options[4] = {"Open", "Save", "Save As", "Exit"};
  int highlight = 0;
  
  
  while(1) { //basically to keep the program running and allow user to choose an option
    
    //print the options (contents of the window): 
    for(int i = 0; i < 4; i++) //4 options
      {
	if(i == highlight) //goes thru i=0, i=1, i=2, i=3. Whatever equals highlight (which changes whenever user presses UP or DOWN) is what gets highlighted
	  wattron(win, A_REVERSE); //turn 'on' the highlighted attribute (A_REVERSE) so that the next printed char is highlighted
	mvwprintw(win, i+1, 1, options[i].c_str()); //params are (window, y, x, * char). Print the cstring at options[i], which is "Open" when i = 0.
	wattroff(win, A_REVERSE); //so that the next thing that's printed isn't highlighted 
      } //for
    
    int choice;
    choice = wgetch(win);
    
    switch(choice)
      {
      case KEY_UP:
	if(highlight != 0)
	  highlight--;
	break;
      case KEY_DOWN:
	if(highlight != 3) //max index 
	  highlight++;
	break;
      default:
	break;
      }
  

    if(choice == '\n') //basically the ENTER key
      break; //leave the while loop
    //^ I guess inside here is where you'd put "if highlight = 0", then do code for open
  


    //go back to beginning of while loop (reprint options)

  }//while
  endwin();
  return 0;
}
