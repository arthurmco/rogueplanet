#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <stdio.h>
#include <stdlib.h> //malloc(), free()
#include <string.h> //strrchr(), strlen()
#include "colony.h"

/* Game save manager */


void savegame_save(colony_t colony, char* filename);
int savegame_load(colony_t* colony, char* filename);
char* get_filename_from_path(char* path);

#endif