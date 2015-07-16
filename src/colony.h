#ifndef COLONY_H
#define COLONY_H

#include "planet.h"
#include <stdlib.h>
#include <time.h>

typedef struct colony {
	char* name;
	unsigned int men_population;
	unsigned int women_population;
	unsigned int hour, day;
	planet_t* planet;
} colony_t;

colony_t* colony_init(unsigned int men, unsigned int woman);
void colony_loop(colony_t*);

#endif