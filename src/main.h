#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>  
#include <ncurses.h>
#include <string.h>
#include <unistd.h> //usleep()
#include "planet.h"
#include "savegame.h"

#include "colony.h"

struct menu {
	char* name;
	int pos;
} main_menu_items[] = {
	{"New Game", 1},
	{"Load Game", 2},
	{"Quit", 3},
};

struct menu ingame_menu_items[] = {
	{"New Game", 1},
	{"Continue Game", 2},
	{"Load Game", 3},
	{"Save Game", 4},
	{"Quit", 5},
};

struct menu build_options[] = {
	{"Military", 1},
	{"Economy", 2},
	{"Cancel", 3},
};


colony_t* user_colony;

void main_exit(int);
void create_title(char* title, char* subtitle);
void create_status(char* title, char* subtitle);
int draw_menu(const char* title, struct menu* menu_items, int size);
void print_teletype(char* format, ...);

void draw_map(int camx, int camy, int w, int h);
void load_game();
void save_game(struct gamefile gf);
void new_game();
void game_loop(int camerax, int cameray);

#endif
