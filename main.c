#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void print_spaceship(int y) 
{
  mvprintw(y/2, 2, "   __");
  mvprintw(y/2+1, 2, "   \\ \\_____");
  mvprintw(y/2+2, 2, "###[==_____>");
  mvprintw(y/2+3, 2, "   /_/  ");
}

void print_meteor() 
{
  mvprintw(17, 85, "   ____");
  mvprintw(18, 85, "__/ c  o\\");
  mvprintw(19, 85, "| o  *   \\ ");
  mvprintw(20, 85, "\\     * _/ ");
  mvprintw(21, 85, " \\_o__ /");
}

void print_splash_screen(int x, int y) 
{
  int printStartY = (int) (y / 2) - 6;
  int printStartX = (int) (x / 2) - 61;

  mvprintw(printStartY, printStartX,      "  /$$$$$$                                         /$$$$$$$$                    /$$                 /$$                    ");  
  mvprintw(printStartY + 1, printStartX,  " /$$__  $$                                       | $$_____/                   | $$                | $$                    ");
  mvprintw(printStartY + 2, printStartX,  "| $$  \\__/  /$$$$$$  /$$$$$$   /$$$$$$$  /$$$$$$ | $$      /$$   /$$  /$$$$$$ | $$  /$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$ ");
  mvprintw(printStartY + 3, printStartX,  "|  $$$$$$  /$$__  $$|____  $$ /$$_____/ /$$__  $$| $$$$$  |  $$ /$$/ /$$__  $$| $$ /$$__  $$ /$$__  $$ /$$__  $$ /$$__  $$");
  mvprintw(printStartY + 4, printStartX,  " \\____  $$| $$  \\ $$ /$$$$$$$| $$      | $$$$$$$$| $$__/   \\  $$$$/ | $$  \\ $$| $$| $$  \\ $$| $$  | $$| $$$$$$$$| $$  \\__/");
  mvprintw(printStartY + 5, printStartX,  " /$$  \\ $$| $$  | $$/$$__  $$| $$      | $$_____/| $$       >$$  $$ | $$  | $$| $$| $$  | $$| $$  | $$| $$_____/| $$      ");
  mvprintw(printStartY + 6, printStartX,  "|  $$$$$$/| $$$$$$$/  $$$$$$$|  $$$$$$$|  $$$$$$$| $$$$$$$$/$$/\\  $$| $$$$$$$/| $$|  $$$$$$/|  $$$$$$$|  $$$$$$$| $$      ");
  mvprintw(printStartY + 7, printStartX,  " \\______/ | $$____/ \\_______/ \\_______/ \\_______/|________/__/  \\__/| $$____/ |__/ \\______/  \\_______/ \\_______/|__/      ");
  mvprintw(printStartY + 8, printStartX,  "          | $$                                                      | $$                                                  ");
  mvprintw(printStartY + 9, printStartX,  "          | $$                                                      | $$                                                  ");
  mvprintw(printStartY + 10, printStartX, "          |__/                                                      |__/                                                  ");

  attron(A_UNDERLINE);
  mvprintw(printStartY + 13, (x / 2) - 5, "Press ENTER");
  attroff(A_UNDERLINE);

  int ch = getch();

  if(ch == 10) {
    clear();
    refresh();
  } else {
    print_splash_screen(x, y);
  }
}

void init(int *x, int *y) 
{
  initscr();

  raw();
  cbreak();

  keypad(stdscr, true);

  noecho();

  getmaxyx(stdscr, *y, *x);

  if(has_colors() == FALSE)
	{	
    endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  curs_set(0);
}

int main() 
{
  int innerX, innerY;

  init(&innerX, &innerY);

  attron(COLOR_PAIR(1) | A_BLINK);

  print_splash_screen(innerX, innerY);

  print_spaceship(innerY);
  print_meteor();

  getch();

  endwin();
}
