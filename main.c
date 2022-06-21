#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

int IS_GAME_RUNNING = 1, 
    FRAMES = 0, 
    METEOR_WIDTH = 7,
    MAXY,
    MAXX,
    SHIPX = 2,
    SHIPY;

void handle_input(int ch) 
{
  switch (ch) {
    case KEY_UP:
      SHIPY--; 
      break;
    case KEY_DOWN:
      SHIPY++; 
      break;
    case KEY_RIGHT:
      return; 
      break;
    default: 
      IS_GAME_RUNNING = 0;
      break;
  }   
}

void print_spaceship() 
{
  mvprintw(SHIPY, SHIPX,   "   __");
  mvprintw(SHIPY+1, SHIPX, "   \\ \\_____");
  mvprintw(SHIPY+2, SHIPX, "###[==_____>");
  mvprintw(SHIPY+3, SHIPX, "   /_/  ");
}

void print_meteor(int x, int y) 
{
  mvprintw(y, x,     "   ____");
  mvprintw(y + 1, x, "__/ c  o\\");
  mvprintw(y + 2, x, "| o  *   \\ ");
  mvprintw(y + 3, x, "\\     * _/ ");
  mvprintw(y + 4, x, " \\_o__ /");
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

void init() 
{
  initscr();

  raw();
  cbreak();

  keypad(stdscr, true);

  noecho();

  getmaxyx(stdscr, MAXY, MAXX);

  SHIPY = MAXY / 2;

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
  init();

  attron(COLOR_PAIR(1) | A_BLINK);

  print_splash_screen(MAXX, MAXY);

  while(IS_GAME_RUNNING) {
    print_spaceship();

    print_meteor(MAXX - (FRAMES + METEOR_WIDTH), 17);

    FRAMES++;

    int ch = getch();

    handle_input(ch);

    clear();
    refresh();
  }

  endwin();
}
