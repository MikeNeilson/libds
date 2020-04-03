/*
 * Array
 *
 * A simple array that keeps track of it's total size and the high element (may not match)
 *
 * you can:
 * 	create
 * 	get
 * 	set
 *	apply a function with foreach
 *
 *
 *
 */

#ifndef DS_ARRAY_H
#define DS_ARRAY_H

#include "common.h"


typedef struct ds_array_s *ds_array_t;


/*
 *	parameters
 *	elem_size - size each byte each element will take up
 *	start_size - starting number of elements 
 *
 *	if ctor is NULL nothing is done to initialize the element
 *	if dtor is not NULL the function will be applied to each element before the call to free
 *	is it assumed that the array will be responsible for freeing all memory
 *
 *	if move is not NULL items put the the list will have ownership transfered, the user should 
 *	then call free on the passed in point, if the element was already part of the array (e.g. you 
 *	got the pointer from get) this is a NOP. If the new element is different the destructor is called
 *	on the existing element.
 *
 *	returns point to a valid ds_array_t structure or NULL
 */
ds_array_t ds_array_create( size_t elem_size, size_t start_size, ... );

/*
 * 
 */
void ds_array_destroy( ds_array_t *array );

void* ds_array_set( ds_array_t array, size_t idx, void *data );
/*
 * 
 */
void* ds_array_get( ds_array_t array, size_t idx );
size_t ds_array_size( ds_array_t array );
size_t ds_array_capacity( ds_array_t array );
/*int  ds_array_grow( ds_array_t array, size_t idx ); */
/*int  ds_array_shrink( ds_array_t array, size_t amount ); */
void* ds_array_foreach( ds_array_t array, ds_traverse_f func, void *user );

#endif

