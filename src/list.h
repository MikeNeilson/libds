#ifndef DS_LIST_H
#define DS_LIST_H
#include "common.h"


typedef struct ds_list_s *ds_list_t;
typedef struct ds_list_node_s *ds_list_node_t;


/**
 *
 */
ds_list_t ds_list_create( size_t elem_size, ... );
/**
 *
 *
 *
 */
void      ds_list_destroy( ds_list_t list );
/*void      ds_list_free_elem( ds_list_t list, void *element, ds_comp );*/

ds_list_node_t ds_list_insert_front( ds_list_t list, void *element );
ds_list_node_t ds_list_insert_back( ds_list_t list, void *element );
void* ds_list_remove( ds_list_t list, void *element, ds_comparator_f func );
void* ds_list_traverse( ds_list_t list, ds_traverse_f func,void *user );


#endif

