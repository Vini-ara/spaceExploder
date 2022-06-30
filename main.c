#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct _Position {
  int x, y;
} Position;

const int METEOR_WIDTH = 10, METEOR_HEIGHT = 4, SHIP_WIDTH = 12, SHIP_HEIGHT = 3;

int IS_GAME_RUNNING = 1, FRAMES = 0, MAXY, MAXX, METEOR_QNT, SPAWN_CHANCE = 5;

Position SHIP, METEORS[30];

void generate_random_meteor() {
  int random = (int) (((double) rand() / RAND_MAX) * 100.0);  

  if(random <= SPAWN_CHANCE) {
    METEOR_QNT++;

    for(int i = 0; i < 30; ++i) {
      if(METEORS[i].x == -1) {
        METEORS[i].x = MAXX - METEOR_WIDTH;
        METEORS[i].y = (int) (((double) rand() / RAND_MAX) * 100.0) % MAXY;
        break;
      }
    }
  }
}

int check_inferior_colision(int y) 
{
  int border = y + METEOR_HEIGHT;
  if(border >= SHIP.y && border <= SHIP.y + SHIP_HEIGHT)
    return 1;

  return 0;
}

int check_superior_colision(int y) 
{
  int border = y;
  if(border <= SHIP.y + SHIP_HEIGHT && border >= SHIP.y)
    return 1;

  return 0;
}

void check_colision(Position meteor) 
{
  if(meteor.x <= SHIP.x + SHIP_WIDTH && (check_inferior_colision(meteor.y) || check_superior_colision(meteor.y)))
    IS_GAME_RUNNING = 0;
}

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
  SHIP.x = 2;

  if(has_colors() == FALSE)
  {	
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  curs_set(0);

  for(int i = 0; i < 30; ++i) 
    METEORS[i].x = -1;
}

int main() 
{
  init();

  attron(COLOR_PAIR(1) | A_BLINK);

  srand(time(NULL));

  print_splash_screen(MAXX, MAXY);

  METEOR_QNT = 0;

  while(IS_GAME_RUNNING) 
  {
    print_spaceship();

    generate_random_meteor();

    mvprintw(0, 0, "%d", FRAMES);

    for(int i = 0; i < 30; ++i) {
      if(METEORS[i].x != -1) {
        print_meteor(METEORS[i].x--, METEORS[i].y);

        check_colision(METEORS[i]);
      }

      if(METEORS[i].x == 0) METEORS[i].x = -1;
    }

    FRAMES++;

    int ch = getch();

    handle_input(ch);

    refresh();
    clear();
  }

  endwin();

  return 0;
}
