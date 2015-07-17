#include "main.h"

/* This is Rogueplanet source code. */

#define PAIR_NORMAL 0
#define PAIR_ERROR 1

#define PAIR_LAND 2
#define PAIR_DIRT 3
#define PAIR_WATER 4

int main(int argc, char** argv){
	if(!initscr()) {
		printf("Error initializing screen.\n");
		exit(1);
	}
	
	if (!has_colors()){
		create_title("RoguePlanet v0.1 [No Colors]", "Menu");
	}
	start_color();

	//Initializate color pairs.
	init_pair(PAIR_NORMAL, COLOR_WHITE, COLOR_BLACK);
	init_pair(PAIR_ERROR, COLOR_RED, COLOR_BLACK);
	init_pair(PAIR_LAND, COLOR_WHITE, COLOR_RED);
	init_pair(PAIR_DIRT, COLOR_WHITE, COLOR_YELLOW);
	init_pair(PAIR_WATER, COLOR_WHITE, COLOR_BLUE);
	
	
	
	
	while (1){
	
		create_title("RoguePlanet v0.1", "Menu");
		int opt = draw_menu("MAIN MENU", main_menu_items, 3);
		
		switch(opt){
			case 1:
				//New game
				new_game();
				break;
			case 2:
				//Load game
				load_game();
				clear();
				noecho();
				break;
			
			case 3: 
				//Exit
				main_exit(0);
				break;
			default:
				printw("Not Implemented");
				
				break;			
		}
	}
	
}

int draw_menu(const char* title, struct menu* menu_items, int size){
	int x,y;
	getmaxyx(stdscr, y, x);
	WINDOW* menu;
	
	if (title != NULL)
		menu = newwin(size+5, (strlen(title)>20) ? strlen(title) : 20, 5, (x/2)-10);
	else
		menu = newwin(size+5, 20, 5, (x/2)-10);
	
		
	wrefresh(menu);

	
	
	int key;
	int selected = 0;
	noecho();
	keypad(menu, TRUE);
	
	
	do {
		//Repaint line
		attron(COLOR_PAIR(PAIR_NORMAL));
	
		int i = 0;
		while(i < size){
						
			if (i == selected) {
				wattron(menu, A_REVERSE);
				mvwprintw(menu, i+1, 3, menu_items[selected].name);
				wattroff(menu, A_REVERSE);
			} else {
				mvwprintw(menu, i+1, 3, menu_items[i].name);
			}
				
			i++;
		}
		box(menu, ACS_VLINE, ACS_HLINE);
		
		if (title != NULL)
			mvwprintw(menu, 0, 5, title);
		
		
		wrefresh(stdscr);
		wrefresh(menu);
		
		
		//Get key
		key = wgetch(menu);
		
		switch(key){
			case KEY_DOWN:
				if (selected+1 >= size)
					selected = 0;
				else
					selected++;
			break;
			case KEY_UP:
				if (selected <= 0)
					selected = size-1;
				else
					selected--;
			break;
		}
		
	} while (key != 10);
	//ENTER was pressed
	
	attroff(COLOR_PAIR(PAIR_NORMAL));
	delwin(menu);
	wrefresh(stdscr);
	doupdate();
	return  menu_items[selected].pos;
}

void create_title(char* title, char* subtitle){
	int title_attr;
	if (has_colors()){
		title_attr = COLOR_PAIR(PAIR_NORMAL) |  A_REVERSE;
	} else {
		title_attr =  A_REVERSE;
	}
	
	attron(title_attr);
	int y,x;
	getmaxyx(stdscr, y, x);
	int i = 0;
	while (i<x){
		mvprintw(0, i++, " ");
	}
		
	
	if (title != NULL)
		mvprintw(0, 0, title);
	
	if (subtitle != NULL)
		mvprintw(0, x-strlen(subtitle), subtitle);	
	
	attroff(title_attr);
}

void create_status(char* title, char* subtitle){
	int title_attr;
	if (has_colors()){
		title_attr = COLOR_PAIR(PAIR_NORMAL) | A_REVERSE;
	} else {
		title_attr = A_REVERSE;
	}
	
	attron(title_attr);
	int y,x;
	getmaxyx(stdscr, y, x);
	int i = 0;
	while (i<x){
		mvprintw(y-1, i++, " ");
	}
		
	
	if (title != NULL)
		mvprintw(y-1, 0, title);
	
	if (subtitle != NULL)
		mvprintw(y-1, x-strlen(subtitle), subtitle);	
	
	attroff(title_attr);
}

void print_teletype(char* format, ...){
	char* ns = (char*)malloc(strlen(format)*2);
	va_list args;
	va_start(args, format);
	vsprintf(ns, format, args);
	va_end(args);
	
	timeout(100);
	
	while (*ns != 0){
		printw("%c", *ns++);
		wrefresh(stdscr);		
		usleep(20000);
		int c = getch();
		
		if (c != ERR){
			printw("%s", ns);
			break;
		}
	}
	
	timeout(-1);
}


static char* introdutory_text = 
"Earth, 2150\n\n \
\tIt's over. \n\tThe planet can't handle the humanity anymore\n\
\tHopefully, the RoguePlanet ship is ready.\n\
\tWe got the 1000 most smart men and the 1000 most \
smart women in the Earth to help us building a colony out there \n\n";

static char* intro_colony = 
"In the bounds of Solar System, 2152\n\n\
\tWe have created a council to choose the colony name\n\
\tAfter some meetings, we defined it.\n\tWill be ";



void new_game(){
	clear();
	create_title("New Game", NULL);
	move(3, 2);
	print_teletype(introdutory_text);
	
	
	mvprintw(15,10,"Press any key to continue");
	
	getch();
	
	
	//Create colony object
	//Init 1000 smartest women and 1000 smartest men
	user_colony = colony_init(1000, 1000); 
	planet_init(user_colony->planet);
	
	
	clear();
	create_title("New Game", NULL);
	move(3, 2);
	print_teletype(intro_colony);
	
	echo();
	attron(A_DIM);  
	printw("[enter colony name]: ");
	attroff(A_DIM);
	
	attron(A_BOLD);
	getstr(user_colony->name);
	attroff(A_BOLD);
		
	//Game starts
	create_status("Creating terrain...", NULL);
	refresh();
	
	terrain_init(user_colony->planet->terrain, 
		user_colony->planet->size_x, user_colony->planet->size_y,
		user_colony->planet->seed); 

	game_loop(-1, -1);
	
}

void save_game(struct gamefile gf){
  timeout(-1);
	clear();
	refresh();
	
	create_title("Save Game", NULL);
	move(3, 2);

	
	printw("Type the path you want to save your colony, or nothing to go back: ");
	move(4, 2);
	
	refresh();
	
	echo();
	attron(A_BOLD);
	char* path = (char*)calloc(sizeof(char), 256);
	getstr(path);
	attroff(A_BOLD);
	
	if (path[0] == '\n' || path[0] == 0){
		return;	
	}
	
	savegame_save(gf, path);
	getch();
	
	
	move(6, 2);
	printw("Game saved. Press any key to return. ");
	noecho();
	refresh();
	getch();
	
	return;
}

void load_game(){
 load_begin:
  timeout(-1);
	clear();
	create_title("Load Game", NULL);
	move(3, 2);

	echo();
	printw("Please type the path of your game save, or nothing to go back: ");
	move(4, 2);
	
	refresh();
	
	attron(A_BOLD);
	char* path = (char*)calloc(sizeof(char), 256);
	scanw("%s", path);
	attroff(A_BOLD);
	
	if (path[0] == '\n' || path[0] == 0){
	  free(path);
	  return;	
	}
	def_prog_mode(); // Save the tty modes		  
	endwin();	 // End curses mode temporarily	  

	printf("Loading terrain %s", path);



	user_colony = (colony_t*)malloc(sizeof(colony_t));
	user_colony->planet = (planet_t*)malloc(sizeof(planet_t));

	struct gamefile gf;
	gf.c = user_colony;
	
	int load = savegame_load(&gf, path);
	
	
	reset_prog_mode(); //Return from state stored by def_prog_mode()
	refresh(); //Do refresh() to restore the screen data
	if (load < 0){
		attron(COLOR_PAIR(PAIR_ERROR)|A_REVERSE|A_BOLD);
		move(5, 2);
		printw("ERROR: ");
		attroff(A_REVERSE);
		
		switch(load){
			case -1:
				printw(" File not found");
				break;
				
			case -2:
				printw(" Invalid file: invalid format");
				break;

		        case -3:
				printw(" Invalid file: file not compressed");
				break;
				
			default:
			 printw(" Unknown %d", load);
				
		}
		
		noecho();
		refresh();
		attroff(A_BOLD|COLOR_PAIR(PAIR_ERROR));
		getch();
		goto load_begin;		
	}
	
	
	move(6, 2);
	printw("Loading terrain from %s", path);
	
	refresh();

	printw(".");

	user_colony->planet->terrain = (unsigned char*)
	  malloc(user_colony->planet->size_x*
		 user_colony->planet->size_y);

	printw(".");
	
	terrain_init(user_colony->planet->terrain, 
		user_colony->planet->size_x, user_colony->planet->size_y,
		user_colony->planet->seed); 

	game_loop(gf.camera_x, gf.camera_y);
}
	
void game_loop(int camera_x, int camera_y){
	char title[64];
	char subt[64];
	
	int camx, camy, camw, camh;
	camx = camera_x == -1 ? rand()%(user_colony->planet->size_x/2)
	  : camera_x;
	camy = camera_y == -1 ? rand()%(user_colony->planet->size_y/2)
	  : camera_y;
	
	getmaxyx(stdscr, camh, camw);
	noecho();
	keypad(stdscr, TRUE);
	timeout(10);

	
	structures_init(user_colony->planet, camx, camy);
	clear();
					
	while(1){
		getmaxyx(stdscr, camh, camw);
		if (camy<0)
			camy=0;
		
		if (camx<0)
			camx=0;
		
		draw_map(camx, camy, camw, camh);
		
		sprintf(title, "RoguePlanet - %s", user_colony->name);
		sprintf(subt, "Population - %d", 
			user_colony->men_population + user_colony->women_population);
		create_title(title, subt);
		
		
		sprintf(title, "Day %d | ESC - Pause | B - Build | P - Colony Properties\t", user_colony->day);
		sprintf(subt, " (X: %5d, Y: %5d)", camx, camy);
		create_status(title, subt);
		int u = 10000;
		
		switch(getch()){
			case KEY_UP:
				camy--;
				break;
			case KEY_DOWN:
				camy++;
				break;
			
			case KEY_LEFT: 
				camx--;
				break;
			
			case KEY_RIGHT:
				camx++;
				break;
			
			case 27:
				//Pause
				switch(draw_menu("PAUSED", ingame_menu_items, 5)){
					case 1: //New Game
						new_game();
						break;
					case 2: //Continue game
						clear();
						break;
					case 3:
						//Load game
						load_game();
						noecho();
						break;
				       case 4: {
					        //Save game
					 struct gamefile gf;
					 gf.camera_x = camx;
					 gf.camera_y = camy;
					 gf.c = user_colony;
					 save_game(gf);
						noecho();
						break;}

  				       case 5:
						main_exit(0);
						break;
					default:
						break;
						
				}
				break;
				
			case 'B':
			case 'b':{
				struct menu bmenu[MAX_STRUCT+1];
				int i = 0;
				
				while (i < MAX_STRUCT){
					bmenu[i].name = structures[i].name;
					bmenu[i].pos = structures[i].SID;
					i++;
				}
				
				bmenu[MAX_STRUCT].name = "Close";
				bmenu[MAX_STRUCT].pos = structures[i-1].SID+1;
				
				int building = draw_menu("Build", bmenu, i+1);
				switch (building){
					case (MAX_STRUCT+1):
						break;
					default: {
						int x, y;
						
						//Launch the Building Positions 'window'
						WINDOW* build = newwin(10, 50, 5, 20);
						echo();
						wbkgd(build, COLOR_PAIR(PAIR_NORMAL));
						box(build, ACS_VLINE, ACS_HLINE);
						
						mvwprintw(build, 0, 5, "Building Positions");
						wmove(build, 2, 3);
						
						wprintw(build, "Type X position of desired building,");
						mvwprintw(build, 3, 3, "or -1 to cancel: ");
						
						wrefresh(stdscr);
						wrefresh(build);
										
						wscanw(build, "%d", &x);
						
						if (x < 0){
							noecho();
							continue;
						}
						
						wmove(build, 5, 3);
						wprintw(build, "Type Y position of desired building,");
						mvwprintw(build, 6, 3, "or -1 to cancel: ");
						
						wrefresh(stdscr);
						wrefresh(build);
						
						wscanw(build, "%d", &y);
						
						if (y < 0){
							noecho();
							continue;
						}
						
						delwin(build);
						wrefresh(stdscr);
						
						//create and add structure to our map
						structure_instance_t instance;
						instance.ID = 0; //will allocate the last ID
						instance.structure = structures[building-1];
						instance.structure.x = x;
						instance.structure.y = y;
						
						//update logic and visual
						structures_add(user_colony->planet->structures, &instance);
						structures_print_on_terrain(user_colony->planet, instance);
						
						noecho();
						}
				break;}
				break;}
				
			case ERR:
				refresh();
				u = 1;
				break;
			
				
		}
		
		usleep(u);
		colony_loop(user_colony);
		
	}
	
	
	
	
	main_exit(0);
	
	
	
}

void draw_map(int camx, int camy, int w, int h){
	int x = camx;
	int y = camy;
	int i = 0;
	int j = 0;
	for (y = camy; y < (camy+h); y++){
	
		for (x = camx; x < (camx+w); x++){
			move(i, j );
			switch (user_colony->planet->terrain[(y*user_colony->planet->size_x)+x]){
				case T_LAND:
					attron(COLOR_PAIR(PAIR_LAND));
					printw("%c", 'W');
					attroff(COLOR_PAIR(PAIR_LAND));
				break;
				case T_DIRT:
					attron(COLOR_PAIR(PAIR_DIRT)|A_BOLD);
					printw("%c", '.');
					attroff(COLOR_PAIR(PAIR_DIRT)|A_BOLD);
				break;
				case T_WATER:
					attron(COLOR_PAIR(PAIR_WATER)|A_BOLD);
					printw("%c", '~');
					attroff(COLOR_PAIR(PAIR_WATER)|A_BOLD);
				break;
				default:{
					//Check if is a building 
					unsigned char terrainID = user_colony->planet->terrain[(y*user_colony->planet->size_x)+x];
					if (terrainID > 16){
						//16 is minimal size to have a building, 
						//becuase 1 << 4 = 10000b = 16, but minimal terrain value is 1,
						//so it's "greater than", not "greater than/equal"
						unsigned int ID = (terrainID >> 4);
						printw("%c", structures[ID-1].print);
						
					} else {				
						printw("%c", '?');
					}
				break;}
			}
			j++;
			
		}
		i++;
	}
	
	
	
}

void main_exit(int code){
	endwin();
	exit(code);
}
	
