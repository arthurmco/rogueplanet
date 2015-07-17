#include "savegame.h"

/* A description of file format 

	It's a gzipped-tarball containing:
		data: the colony&planet info
		structures: structure info (NOT IMPLEMENTED NOW)
*/

int savegame_save(struct gamefile game, char* filename){

  colony_t colony = *game.c;
  
	//Start saving colony info
	FILE* f_colony = fopen("data", "w");
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
	
	//compress using tar. 
	char* cmdargs[] = {"/bin/tar", "-zcf",
			   filename, "data", NULL};


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
        
	char datafile[128];
	sprintf(datafile, "%sdata", defaultextract);
	
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
	
	return 0;
	
}
