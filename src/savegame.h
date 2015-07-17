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

struct gamefile {
  colony_t* c;
  int camera_x, camera_y;
};

  int savegame_save(struct gamefile game, char* filename);
int savegame_load(struct gamefile* game, char* filename);
char* get_filename_from_path(char* path);

#endif
