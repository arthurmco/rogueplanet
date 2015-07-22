#include "savegame.h"

/* A description of file format 

	It's a gzipped-tarball containing:
		data: the colony&planet info
		structures: structure info (NOT IMPLEMENTED NOW)
*/

int savegame_save(struct gamefile game, char* filename){

  colony_t colony = *game.c;
  
	//Start saving colony info
	FILE* f_colony = fopen("/tmp/data", "w");
	fprintf(f_colony, "colony.name: %s\n", colony.name);
	fprintf(f_colony, "colony.men_population: %d\n", colony.men_population);
	fprintf(f_colony, "colony.women_population: %d\n", colony.women_population);
	fprintf(f_colony, "colony.day: %d:%d\n", colony.day, colony.hour);
	
	//Now planet info
	fprintf(f_colony, "planet.size: %dx%d\n", colony.planet->size_x, colony.planet->size_y); 
	fprintf(f_colony, "planet.height: %d\n", colony.planet->height); 
	fprintf(f_colony, "planet.seed: %d\n", colony.planet->seed);

	//Game info
	fprintf(f_colony, "game.camera: %d %d\n", game.camera_x, game.camera_y);
	fflush(f_colony);
	fclose(f_colony);

	//Now save all structures
	FILE* f_structures = fopen("/tmp/structs", "w");

	structures_list_t* struct_item = colony.planet->structures;
	fprintf(f_structures, "# Instance StructID X Y\n");
	do {
	  //Instance ID, Structure ID, X Pos, Y Pos

	  fprintf(f_structures, "%d %d %d %d\n",
		  struct_item->instance_structure.ID,
		  struct_item->instance_structure.structure.SID,
		  struct_item->instance_structure.structure.x,
		  struct_item->instance_structure.structure.y);		  
	  
	  struct_item = struct_item->next_structure;
	} while (struct_item != NULL);

	fflush(f_structures);
	fclose(f_structures);
	
	//compress using tar.
	chdir("/tmp");
	char* cmdargs[] = {"/bin/tar", "-zcf", filename,
			   "data", "structs", NULL};


	pid_t tar_pid;
	tar_pid = fork(); //fork the process...
	int tar_completed = 0;

	if (tar_pid == -1){
	  return -4;
	}

	if (tar_pid == 0){
	  //and change it to tar
	  execv(cmdargs[0], cmdargs);
	}

	//wait for result
	int res = -1;

 wait_proc:
	if (waitpid(tar_pid, &res, 0) == -1){
	  return -4; //maybe tar not found?
	}

	if (WIFEXITED(res)){
	  //check error code
	  int status = WEXITSTATUS(res);
	  if (status != 0) {
	    return -3;
	    } else {
	    return -3;
	  }
	} else {
	  goto wait_proc; //hasn't exited, loop until it exits.
	}

	
	
}

char* get_filename_from_path(char* path){
	//Allocate for our worst case: given path is filename
	char* file = (char*)malloc(strlen(path));
	char* relative;
		
	#ifdef _WIN32
		char pathsep = '\\'; //why you're different Windows?
	#else
		char pathsep = '/';
	#endif
	
	relative = strrchr(path, pathsep);
	
	if (relative == NULL){
		free(file);	 //we have no slashes. This mean that we
		return path; //have a relative path.
	}
	file = strncpy(file, ++relative, strlen(relative));
	
	return file;
		
}

/* Load a game. Return:
 * 0 if success
 * -1 if file wasn't found
 * -2 if error while parsing file
 * -3 if could not uncompress */
int savegame_load(struct gamefile* game, char* filename){

  colony_t* colony = game->c; 
  
	char tmpfolder[] = "/tmp/";
	char defaultextract[strlen(tmpfolder)+strlen(filename)+1];
	
	sprintf(defaultextract, "%s%s/", tmpfolder, get_filename_from_path((filename)));	

	mkdir(defaultextract, 0777);
	//untar file in the specified folder
	char* cmdargs[] = {"/bin/tar", "-zxf",
			   filename, "-C", defaultextract, NULL};


	pid_t tar_pid;
	tar_pid = fork(); //fork the process...
	int tar_completed = 0;

	if (tar_pid == -1){
	  return -4;
	}

	if (tar_pid == 0){
	  //and change it to tar
	  execv("/bin/tar", cmdargs);
	}

	//wait for result
	int res = -1;

 wait_proc:
	if (waitpid(tar_pid, &res, 0) == -1){
	  return -4; //maybe tar not found?
	}

	if (WIFEXITED(res)){
	  //check error code
	  int status = WEXITSTATUS(res);
	  if (status != 0) {
	    return -3;
	  }
	} else {
	  goto wait_proc; //hasn't exited, loop until it exits.
	}
        
	char datafile[128], structfile[128];
	sprintf(datafile, "%sdata", defaultextract);
	sprintf(structfile, "%sstructs", defaultextract);
	
	FILE* f_data = fopen(datafile, "r");
	
	
	if (f_data == NULL){
		return -1;	
	}
	char* line = (char*)malloc(256);
	
	while(!feof(f_data)){
		fgets(line, 255, f_data);
		
		//inner loop created to remove unwanted characters
		while (*line != NULL){
			if (*line == ' '){
				line++;	
				continue;
			}
			
			if (*line == '#'){
				//Commen_populationt
				break;
			}
			
			
			//each line is composed by a format like property:value.
			//So they're pretty easy to parse
			char* colon = strchr(line, ':');
			
			if (colon == NULL){
				//no colon on a valid line? bad file.
				fclose(f_data);
				return -2;	
			}
			
			int p_length = (int)(colon-line);
			char* p_value = ++colon;
			
			#define IS_PROPERTY(p) (strncmp(line, p, p_length) == 0)
			
			if(IS_PROPERTY("colony.name")){
				colony->name = (char*)malloc(strlen(p_value)+1);
				strcpy(colony->name, p_value);
				break; //next line
			}
			
			if(IS_PROPERTY("colony.men_population")){
				colony->men_population = atoi(p_value); 
				break; //next line
			}
			
			if(IS_PROPERTY("colony.women_population")){
				colony->women_population = atoi(p_value); 
				break; //next line
			}
			
			if(IS_PROPERTY("colony.day")){
				sscanf(p_value, "%d:%d", &colony->day, &colony->hour);
				break;
			}
			
				
			if(IS_PROPERTY("planet.height")){
				colony->planet->height = atoi(p_value); 
				break; //next line
			}
			
			if(IS_PROPERTY("planet.seed")){
				colony->planet->seed = atoi(p_value); 
				break; //next line
			}
			
			if(IS_PROPERTY("planet.size")){
				sscanf(p_value, "%dx%d", &(colony->planet->size_x), &(colony->planet->size_y));
				break;
			}

			if(IS_PROPERTY("game.camera")){
			  sscanf(p_value, "%d %d", &(game->camera_x), &(game->camera_y));
				break;
			}
		
		}
		
		
	}

	/*** Now get the structure ***/
	
	FILE* f_struct = fopen(structfile, "r");


	if (line != NULL)
	  free(line);
	  
	//We don't need much for this one.
	line = malloc(32*sizeof(char)); 

	//The first element will begin our structure
	structures_init(colony->planet, 0, 0);

	do {
	  fgets(line, 31, f_struct);

	} while (line[0] != '#');

	/* Get first element, separated from others
	   Since the first element is the list handle himself,
	   the method of creating him is different */ 
	unsigned int SID = 0, x = 0, y = 0;
	  
	  fscanf(f_struct, "%u %u %u %u",
		 &(colony->planet->structures->instance_structure.ID),
		 &SID, &x, &y);

	  //Get all information of structure
	  memcpy(&colony->planet->structures->instance_structure.structure,
		 &structures[SID-1], sizeof(structure_t));

	  colony->planet->structures->instance_structure.structure.x = x;
  	  colony->planet->structures->instance_structure.structure.y = y;
	  //Get new line.
	  
	
	  structures_list_t* sl_prev = colony->planet->structures;
	while (!feof(f_struct)){
	  fgets(line, 31, f_struct);
	  int charindex = 0;
	analyze_char:

	  if (charindex > 31)
	    continue;
	  
	  switch (line[charindex]){
	  case '#': //comment
	    continue;
	    
	  case ' ': //space
	    charindex++;
	    goto analyze_char;
	    break;

	  }	 
	  
	  structures_list_t* sl = malloc(sizeof(structures_list_t));
	  sl->prev_structure = sl_prev;
	  sl_prev->next_structure = sl;
	  sl->next_structure = NULL;
	  
	  sscanf(line, "%u %u %u %u",
		 &(sl->instance_structure.ID),
		 &SID, &x, &y);

	  //Get all information of structure
	  memcpy(&sl->instance_structure.structure,
		 &structures[SID-1], sizeof(structure_t));

	  sl->instance_structure.structure.x = x;
  	  sl->instance_structure.structure.y = y;	  

	  sl_prev = sl;

	}
	free(line);
	return 0;
	
}
