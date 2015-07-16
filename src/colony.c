#include "colony.h"

colony_t* colony_init(unsigned int men, unsigned int woman){
	
	colony_t* user_colony;
	user_colony = (colony_t*)malloc(sizeof(colony_t));
	user_colony->men_population = men; //The population
	user_colony->women_population = woman;
	user_colony->name = (char*)malloc(256);
	user_colony->planet = (planet_t*)malloc(sizeof(planet_t));
	user_colony->hour = 0;
	user_colony->day = 0;
		
	srand(time(NULL));
	
	return user_colony;
}

void colony_loop(colony_t* c){
	//Each iteration of this loop means one day.
	
	//Chances of sexual interation
	int fuck = (rand() % 20000);
	
	if (fuck < 100)
		c->men_population++;
	
	if (fuck > 102 && fuck < 200)
		c->women_population++;
	
	if (fuck >= 100 && fuck <= 102){
		c->men_population++;
		c->women_population++;
	}
	
	c->hour++;
	
	if (c->hour > 24){
		c->hour = 0;
		c->day++;
	}
}