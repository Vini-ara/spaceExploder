#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct _Position {
  int x, y;
} Position;

/* Global Variabales */
const int METEOR_WIDTH = 10, METEOR_HEIGHT = 4, SHIP_WIDTH = 12, SHIP_HEIGHT = 3;

int IS_GAME_RUNNING = 1, FRAMES = 0, MAXY, MAXX, METEOR_QNT, SPAWN_CHANCE = 20, FIRE_RATE = 30;

Position SHIP, METEORS[30], PROJECTILES[100];

/*
 * generates a new projectile at the first free position of the projectiles vector
 */
void fire_projectile() {
  int starting_pos_y = SHIP.y + 2; // center of the ship
  int starting_pos_x = SHIP.x + SHIP_WIDTH;

  for(int i = 0; i < 100; ++i) {
    if(PROJECTILES[i].x == -1) {
      PROJECTILES[i].x = starting_pos_x;
      PROJECTILES[i].y = starting_pos_y;
      break;
    }
  }
}

void check_projectile_colision(int x, int y, int index) {
  for(int i = 0; i < 100; ++i) {
    int projectile_x = PROJECTILES[i].x;
    int projectile_y = PROJECTILES[i].y;

    if(projectile_x >= x && (projectile_y >= y && projectile_y <= y + METEOR_HEIGHT)) {
      PROJECTILES[i].x = -1;
      METEORS[index].x = -1;
      break;
    }
  }
}

void print_projectile(int x, int y) {
  attroff(COLOR_PAIR(1));
  
  attron(COLOR_PAIR(2));
  mvaddch(y, x, '0');
  attroff(COLOR_PAIR(2));

  attron(COLOR_PAIR(1));
}

/*
 * uses Rand function to determinate if a meteor should be printed and in wihch position
 */
void generate_random_meteor() {
  int random = (int) (((double) rand() / RAND_MAX) * 100.0);  

  if(random <= SPAWN_CHANCE) {
    for(int i = 0; i < 30; ++i) {
      if(METEORS[i].x == -1) {
        METEORS[i].x = MAXX - METEOR_WIDTH;
        METEORS[i].y = (int) (((double) rand() / RAND_MAX) * 100.0) % (MAXY - METEOR_HEIGHT);
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

/*
 * Checks whether the ship is coliding with the meteor passed as argument
 */
void check_ship_colision(Position meteor) 
{
  if(meteor.x <= SHIP.x + SHIP_WIDTH && (check_inferior_colision(meteor.y) || check_superior_colision(meteor.y)))
    IS_GAME_RUNNING = 0;
}

/* 
 * Process all the key inputs done during game
 */
void handle_input(int ch) 
{
  switch (ch) {
    case KEY_UP:
      if(SHIP.y > 0) SHIP.y--; 
      return;
      break;
    case KEY_DOWN:
      if(SHIP.y < (MAXY - METEOR_HEIGHT)) SHIP.y++;
      return;
      break;
    case KEY_RIGHT:
      return; 
      break;
    default: 
      return;
      break;
  }   
}

/*
 * Spaceship printing function
 */
void print_spaceship() 
{
  mvprintw(SHIP.y, SHIP.x,     "   __");
  mvprintw(SHIP.y + 1, SHIP.x, "   \\ \\_____");
  mvprintw(SHIP.y + 2, SHIP.x, "###[==_____>");
  mvprintw(SHIP.y + 3, SHIP.x, "   /_/  ");
}

/*
 * Meteor printing function 
 */
void print_meteor(int x, int y) 
{
  mvprintw(y, x,     "   ____");
  mvprintw(y + 1, x, "__/ c  o\\");
  mvprintw(y + 2, x, "| o  *   \\ ");
  mvprintw(y + 3, x, "\\     * _/ ");
  mvprintw(y + 4, x, " \\_o__ /");
}

/*
 * home Screen Printing Function 
 */
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

  attron(A_UNDERLINE | A_BLINK);
  mvprintw(printStartY + 13, (x / 2) - 5, "Press ENTER");
  attroff(A_UNDERLINE | A_BLINK);

  int ch = getch();

  if(ch == 10) {
    clear();
    refresh();
  } else {
    print_splash_screen(x, y);
  }
}

/* 
 * Intialization deatails referent to the ncurses library, and setting some basic game 
 * logic intial settings
*/
void init() 
{
  initscr();

  srand(time(NULL));

  raw();
  noecho();

  halfdelay(1);

  keypad(stdscr, true);

  if(has_colors() == FALSE)
  {	
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(1));

  getmaxyx(stdscr, MAXY, MAXX);

  curs_set(0);

  SHIP.y = MAXY / 2;
  SHIP.x = 2;

  // sets a initial value to the x of the position arrays
  for(int i = 0; i < 30; ++i) 
    METEORS[i].x = -1;

  for(int i = 0; i < 100; ++i)
    PROJECTILES[i].x = -1;
}

int main() 
{
  init();

  print_splash_screen(MAXX, MAXY);

  while(IS_GAME_RUNNING) 
  {
    print_spaceship();

    generate_random_meteor();


    for(int i = 0; i < 30; ++i) {
      if(METEORS[i].x != -1) {
        print_meteor(METEORS[i].x, METEORS[i].y);

        METEORS[i].x -= 5;
        
        check_ship_colision(METEORS[i]);
        check_projectile_colision(METEORS[i].x, METEORS[i].y, i);
      }

      if(METEORS[i].x == 0) METEORS[i].x = -1;
    }

    if(!(FRAMES % FIRE_RATE)) fire_projectile();

    for(int i = 0; i < 100; ++i) {
      if(PROJECTILES[i].x != -1) {
        print_projectile(PROJECTILES[i].x++, PROJECTILES[i].y);

      }

      if(PROJECTILES[i].x == MAXX) PROJECTILES[i].x = -1;
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
