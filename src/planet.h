#ifndef PLANET_H
#define PLANET_H

#include "structures.h"
#include <stdlib.h>
#include <string.h> //for memcpy()

/*	Elements */
	enum Gases {
		G_OXYGEN = 0x01,
		G_CARBON_MONOXIDE = 0x02,
		G_CARBON_DIOXIDE = 0x04,
		G_SULFUR = 0x08,
		G_HYDROGEN = 0x0f,
	};
	
/* Terrain types */
	enum Terrain {
		T_LAND = 1,
		T_DIRT = 2,
		T_WATER = 3,
		
		
		T_LOWELEVATION = 8,
		T_HIGHELEVATION = 16,
		T_BUILDING = 32
	};
	
/* Atmosphere definitions. Not used yet */
typedef struct atmosphere {
	struct atmosphere* prev_gas;
	unsigned int gas;	  
	unsigned int amount; //Amount maximum is 100
	struct atmosphere* next_gas;
} atmosphere_t;
	

/* 	Planet definitions
	You will land into a random planet.
	These structures will define the planet info */
typedef struct planet {
	unsigned int size_x;
	unsigned int size_y;
	unsigned int height; //planet height, from land to space
	atmosphere_t* atmosphere;
	structures_list_t* structures;
	unsigned char* terrain;
	unsigned int seed;
} planet_t;

void planet_init(planet_t* planet);
void terrain_init(unsigned char* terrain, int w, int h, 
	unsigned int seed);

void structures_init(planet_t* planet, unsigned int startx, unsigned int starty);
void structures_print_on_terrain(planet_t* planet, structure_instance_t s);

#endif
