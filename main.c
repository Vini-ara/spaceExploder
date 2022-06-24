#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Position {
  int x, y;
} Position;

const int METEOR_WIDTH = 10;

int IS_GAME_RUNNING = 1, FRAMES = 0, MAXY, MAXX, METEOR_QNT;

Position SHIP, METEORS[30];

void handle_input(int ch) 
{
  switch (ch) {
    case KEY_UP:
      SHIP.y--; 
      break;
    case KEY_DOWN:
      SHIP.y++; 
      break;
    case KEY_RIGHT:
      return; 
      break;
    default: 
      return;
      break;
  }   
}

void print_spaceship() 
{
  mvprintw(SHIP.y, SHIP.x,     "   __");
  mvprintw(SHIP.y + 1, SHIP.x, "   \\ \\_____");
  mvprintw(SHIP.y + 2, SHIP.x, "###[==_____>");
  mvprintw(SHIP.y + 3, SHIP.x, "   /_/  ");
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
    printw("%d", ch);
  }
}

void init() 
{
  initscr();

  raw();
  halfdelay(2);

  keypad(stdscr, true);

  noecho();

  getmaxyx(stdscr, MAXY, MAXX);

  SHIP.y = MAXY / 2;

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

  METEORS[0].x = MAXX - (FRAMES + METEOR_WIDTH);
  METEORS[0].y = 17;

  METEOR_QNT = 1;

  while(IS_GAME_RUNNING) 
  {
    print_spaceship();

    for(int i = 0; i < METEOR_QNT; ++i) {
      if(METEORS[i].x == 0) {
        METEORS[i].x = MAXX - METEOR_WIDTH;
      }
      print_meteor(METEORS[i].x--, METEORS[i].y);
    }

    print_meteor(MAXX - (FRAMES + METEOR_WIDTH), 17);

    FRAMES++;

    int ch = getch();

    handle_input(ch);

    clear();
    refresh();
  }

  endwin();
}
