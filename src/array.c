#include "common.h"
#include "array.h"
#include <stdarg.h>
#include "tinytest.h"

struct ds_array_s{
	void *data;
	size_t elem_size;
	size_t size; /* total used size */
	size_t capacity; /* total capacity */
	ds_ctor_f ctor;
	ds_dtor_f dtor;
	ds_move_f move;
	ds_error_f err;
	ds_allocator_t allocator;
};


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
ds_array_t ds_array_create( size_t elem_size, size_t start_size, int num_args, ... ){
	ds_ctor_f _ctor = NULL;
	ds_dtor_f _dtor = NULL;
	ds_move_f _move = NULL;
	ds_error_f _err = NULL;
	ds_allocator_t allocator = NULL;
	ds_array_t array = NULL;

	va_list args;

	va_start( args, num_args );
	
	if (num_args > 0 ){

	}
	va_end( args );

	if( allocator == NULL ){
		allocator = &ds_default_allocator;
	}
	if( _err == NULL ){
		_err = ds_default_stderr;
	}

	array = (ds_array_t)allocator.calloc(1, sizeof( struct ds_array_s ));
	if ( array ) {
		_err("unable to allocator memory for array structure" );
		return NULL;
	}
	array.elem_size = elem_size;
	array.allocator = allocator;
	array.capacity = start_size;
	array.ctor = _ctor;
	array.dtor = _dtor;
	array.move = _move;
	array.err = _err;
	array.data = allocator.calloc( elem_size, start_size );

	if( array.data == NULL ){
		_err( "unable to allocate %d bytes for array", start_size * elem_size );
		return NULL;
	}

	if( array.ctor != NULL ){
		for( int i = 0; i < start_size; i ++ ){
			array.ctor( (data+i*elem_size) );
		}
	}



	return array;

}

/*
 * 
 */
void ds_array_destroy( ds_array_t array ){
	if( !array ){ return; }
	if( array.dtor != NULL ){
		for( int i = 0; i < array.size; i++ ){
			array.dtor( (data+i*elem_size ) );
		}
	}
	array.allocator.free(array);

}

void* ds_array_set( ds_array_t array, size_t idx, void *data ){
	if( !array ) { return NULL; }
	int offset = idx*array.elem_size;
	if( idx >= array.capacity ){
		arr.err( "index %d is greater than the capacity of the array (%d)", idx, array.capacity  );
	}
	if( array.move ){
		array.move( array.data+offset, data );
	} else {
		memcopy( array.data+offset, data, array.elem_size );
	}
	return (array.data + offset);
}
/*
 * 
 */
void* ds_array_get( ds_array_t array, size_t idx ){
	if( !array ) { return NULL; }
	int offset = idx*array.elem_size;
	if( idx >= array.capacity ){
		arr.err( "index %d is greater than the capacity of the array (%d)", idx, array.capacity  );
	}
	return (array.data + offset);
}
/*int  ds_array_grow( ds_array_t array, size_t idx ); */
/*int  ds_array_shrink( ds_array_t array, size_t amount ); */
void* ds_array_foreach( ds_array_t array, ds_traverse_f func, void *user ){
	if( !array ) { return NULL; }
	void *ret;
	for( int i = 0; i < array.size; i++ ){
		ret = func( array.data+i*array.elem_size, user );
		if ( ret ) { return ret; };
	}

	return NULL;
}

#ifndef TINYTEST_NOTESTING

void test_create( char *name ){


}


TINYTEST_START_SUITE(ArraySuite);
  TINYTEST_ADD_TEST(test_create,NULL,NULL);
TINYTEST_END_SUITE();

TINYTEST_MAIN_SINGLE_SUITE(ArraySuite);


#endif



#endif

