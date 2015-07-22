#include "structures.h"

void dummy_iter(struct structure* s){

}

void structures_add(structures_list_t* list, structure_instance_t* s){
  structures_list_t* last_item = list;

  int lid = 0;

  while (last_item->next_structure != NULL){
    last_item = last_item->next_structure;
    lid++;
  }
        structures_list_t* newitem = (structures_list_t*)malloc(sizeof(structures_list_t));
	

	newitem->prev_structure = last_item;
	newitem->next_structure = NULL;
	last_item->next_structure = newitem;
	s->ID = lid;
	newitem-> instance_structure = *s;
	newitem-> instance_structure.ID = list-> instance_structure.ID+1;
	
	
	
}

void structures_remove(structures_list_t* list, structure_instance_t* s){
}
