#include "savegame.h"

/* A description of file format 

	It's a gzipped-tarball containing:
		data: the colony&planet info
		structures: structure info (NOT IMPLEmen_populationTED NOW)
*/

void savegame_save(colony_t colony, char* filename){
	
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
	
	fflush(f_colony);
	
	//compress using tar.
	
	//28 is more or less the lenght of "tar -zcf ... data structures "
	char command[strlen(filename)+28];
	sprintf(command, "tar -zcf %s data", filename);
		
	
	
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
 * -2 if error while parsing file */
int savegame_load(colony_t* colony, char* filename){
	char tmpfolder[] = "/tmp/";
	char defaultextract[strlen(tmpfolder)+strlen(filename)+1];
	
	sprintf(defaultextract, "%s%s/", tmpfolder, get_filename_from_path((filename)));
	
	char command[strlen(defaultextract)+28];
	sprintf(command, "tar -zxf %s %s", filename, defaultextract);
	
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
		while (*line == NULL){
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
		
		}
		
		
	}
	
	return 0;
	
}