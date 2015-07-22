#include "planet.h"


void planet_init(planet_t* planet){
	planet->size_x = rand() % 16384;
	planet->size_y = rand() % 16384;
	
	planet->terrain = (unsigned char*)malloc(
		planet->size_x * planet->size_y);
	
	planet->seed = rand();
	
	
	
	
}

/** Initialize the structure and creates a colony center
 * planet = pointer to the planet that structures will be in
 *  startx = X Starting point, or 0 if you don't want any colony center
 *  starty = Y Starting point, or 0 if... 
 **/
void structures_init(planet_t* planet, unsigned int startx, unsigned int starty){

	//Create a colony center where you started
	planet->structures = (structures_list_t*)malloc(sizeof(structures_list_t));
	planet->structures->prev_structure = NULL;
	planet->structures->next_structure = NULL;
	planet->structures->instance_structure.ID = 0;
	memcpy(&planet->structures->instance_structure.structure, &structures[0], sizeof(structure_t));
	planet->structures->instance_structure.structure.x = startx + 5;
	planet->structures->instance_structure.structure.y = starty + 5;

	
	if (startx > 0 && starty > 0)
	  structures_print_on_terrain(planet, planet->structures->instance_structure);
}

void structures_print_on_terrain(planet_t* planet, structure_instance_t s){
	
	unsigned int x,y,w,h;

	x = s.structure.x;
	y = s.structure.y;
	w = x+s.structure.size_x;
	h = y+s.structure.size_y;
	
	while(y<h){
		while (x<w){
			
			planet->terrain[(y*planet->size_x)+x] |= (s.structure.SID << 4); 			
			x++;
		}
		y++;
		x = s.structure.x;
	}

}


void terrain_init(unsigned char* terrain, int w, int h,
	unsigned int seed){
	int p = 0;
	 int size = (w*h);

	 srand(seed);
	int a = rand()%100;
		
	while(p < size){
		terrain[p] = ((rand()%100) > (a-(rand()%4))) ? T_WATER : T_LAND;
		
		if (terrain[p] == T_WATER){
			if (terrain[p-1] != T_WATER)
				terrain[p-1] = T_DIRT;
			
			if (terrain[p+1] != T_WATER)
				terrain[p+1] = T_DIRT;
			
			if (p>(h+1)){
				if (terrain[p-h-1] != T_WATER)
					terrain[p-h-1] = T_DIRT;
			}
			
			if ((p+h+1)<size){
				if (terrain[p+h+1] != T_WATER)
					terrain[p+h+1] = T_DIRT;
			}
		}
		
		
		
				
		p++;
		
	}
	
	
	
}
