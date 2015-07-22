#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h> //malloc()

/*	Define the planet structures */
typedef struct structure {
	unsigned int SID;
	const char* name;
	unsigned int x, y;
	unsigned int size_x, size_y;
	char print; unsigned char fg; unsigned char bg; char pad;
	void (*iteration)(struct structure* s);
} structure_t;


void dummy_iter(struct structure* s);

static structure_t structures[] = {
	//SID, Name, X, Y, Width, Height, Character shown, Foreground, Background, Padding and Function
	{1, "Colony Center", 0, 0, 10, 10, 'C', 2, 7, ' ', dummy_iter},	
	{2, "Colony Ship", 0, 0, 4, 4, 'S', 2, 7, ' ', dummy_iter},
	{3, "Small Habitation", 0, 0, 1, 1, 'H', 2, 7, ' ', dummy_iter},
	{4, "Big Habitation", 0, 0, 2, 2, 'H', 2, 7, ' ', dummy_iter},
	{5, "Garden", 0, 0, 3, 3, 'G', 2, 7, ' ', dummy_iter},
	{6, "Storage Building", 0, 0, 3, 3, 's', 2, 7, ' ', dummy_iter},
	
};

#define MAX_STRUCT 6


/*  Structure instance */
typedef struct structure_instance {
	int ID;
	structure_t structure;	
} structure_instance_t;

/* A basic double-linked list of all structures of a planet */
typedef struct structures_list {
	struct structures_list* prev_structure;
	structure_instance_t instance_structure;
	struct structures_list* next_structure;
} structures_list_t;

void find_structure(structures_list_t* list, structure_instance_t* s);
void structures_add(structures_list_t* list, structure_instance_t* s);
void structures_remove(structures_list_t** s);


#endif
