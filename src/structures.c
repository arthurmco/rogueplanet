#include "structures.h"

void dummy_iter(struct structure* s){

}

void structures_add(structures_list_t* list, structure_instance_t* s){
	structures_list_t* newitem = (structures_list_t*)malloc(sizeof(structures_list_t));
	newitem->prev_structure = list;
	newitem->next_structure = NULL;
	list->next_structure = newitem;
	newitem-> instance_structure = *s;
	newitem-> instance_structure.ID = list-> instance_structure.ID+1;
	
	
	
}

void structures_remove(structures_list_t* list, structure_instance_t* s){
}