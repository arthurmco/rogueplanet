#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <stdio.h>
#include <stdlib.h> //malloc(), free()
#include <string.h> //strrchr(), strlen()
#include "colony.h"

#include <unistd.h> //fork() and execv()
#include <sys/types.h> 
#include <sys/wait.h> //waitpid()
#include <sys/stat.h>

/* Game save manager */


int savegame_save(colony_t colony, char* filename);
int savegame_load(colony_t* colony, char* filename);
char* get_filename_from_path(char* path);

#endif
