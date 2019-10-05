#ifdef DS_LIST_H
#define DS_LIST_H
#include "common.h"

typedef struct ds_list_node_s{
	struct ds_list_node_s *next;
	void *data;
} ds_list_node_t*;

struct ds_list_s{
	ds_list_node_t head;
	ds_list_node_t tail;
	ds_copy_f copy;
	ds_copy_f move;
	size_t elem_size;

};

ds_list_t ds_list_create( size_t elem_size, ... ){
	
}



void      ds_list_destory( ds_list_t list ){

}
void      ds_list_free_elem( ds_list_t list, void *element ){

}

void* ds_list_insert_front( ds_list_t list, void *element ){
	return NULL;
}

void* ds_list_insert_back( ds_list_t list, void *element ){
	return NULL;
}

void*  ds_list_remove( ds_list_t list, void *element, ds_compare_f func ){
	return NULL;
}

void* ds_list_traverse( ds_list_t list, ds_traverse_f func ){
	return NULL;
}


#endif

