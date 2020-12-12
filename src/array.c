#include <string.h>
#include <stdarg.h>
#include "common.h"
#include "array.h"

struct ds_array_s{
	void *data;
	size_t elem_size;
	size_t size; /* total used size */
	size_t capacity; /* total capacity */
	ds_ctor_f ctor;
	ds_dtor_f dtor;
	ds_copy_f copy;
	ds_move_f move;
	ds_error_f err;
	ds_allocator_t* allocator;
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
ds_array_t ds_array_create( size_t elem_size, size_t start_size, ... ){
	ds_ctor_f _ctor = NULL;
	ds_dtor_f _dtor = NULL;
	ds_move_f _move = NULL;
	ds_copy_f _copy = NULL;
	ds_error_f _err = ds_error_stderr;
	ds_allocator_t* allocator = NULL;
	ds_array_t array = NULL;
	int arg_type=DS_END;
	int i = 0;

	va_list args;

	va_start( args, start_size );
		/* TODO: process the arguments */
		arg_type = va_arg( args, int );
		while( arg_type != DS_END ){
			switch( arg_type ){
				case DS_FUNC_CTOR:
					_ctor = va_arg(args, ds_ctor_f);
					break;
				case DS_FUNC_DTOR:
					_dtor = va_arg(args, ds_dtor_f);
					break;
				case DS_TYPE_ALLOCATOR:
					allocator = va_arg(args, ds_allocator_t* );
					break;
				case DS_FUNC_ERR:
					_err = va_arg(args, ds_error_f);
					break;
				case DS_FUNC_COPY:
					_copy = va_arg(args,ds_copy_f);
				default:
					break;
			}
			arg_type = va_arg(args, int );
		}
	va_end( args );

	if( allocator == NULL ){
		allocator = &ds_default_allocator;
	}
	if( _err == NULL ){
		_err = ds_error_stderr;
	}

	if( _copy == NULL && _move == NULL ){
		_err("You must provide a copy or move function");
		return NULL;
	}

	array = (ds_array_t)allocator->calloc(1, sizeof( struct ds_array_s ));
	if ( !array ) {
		_err("unable to allocate memory for array structure" );
		return NULL;
	}
	array->elem_size = elem_size;
	array->allocator = allocator;
	array->capacity = start_size;
	array->size = 0;
	array->ctor = _ctor;
	array->dtor = _dtor;
	array->move = _move;
	array->copy = _copy;
	array->err = _err;
	array->data = allocator->calloc( elem_size, start_size );

	if( !array->data ){
		_err( "unable to allocate %d bytes for array", start_size * elem_size );
		return NULL;
	}

	if( array->ctor != NULL ){
		for( i = 0; i < start_size; i ++ ){			
			//void *data = array->data;
			char *element = (char*)array->data+(i*elem_size);			
			array->ctor( (void*)element, allocator );
		}
	}



	return array;

}


/*
 * 
 */
void ds_array_destroy( ds_array_t *arr ){
	int i = 0;
	ds_array_t array = *arr;
	if( !array ){ return; }
	if( array->dtor != NULL ){
		for( i = 0; i < array->capacity; i++ ){
			char *element = (char*)array->data+(i*array->elem_size);
			array->dtor( (void*)(element) , array->allocator );
		}
	}
	array->allocator->free( array->data );
	array->allocator->free(*arr);
	if( (*arr) ){ *arr = NULL; }

}

void ds_array_grow( ds_array_t array, size_t amount ){
	size_t old_cap = array->capacity;
	size_t i = 0;
	array->data = array->allocator->realloc( array->data, (old_cap+amount)*array->elem_size );
	/* TODO: run constructor on new memory */
	char *bytes = (char*)array->data;
	memset( bytes+array->elem_size*old_cap,0, amount*array->elem_size );
	if( array->ctor != NULL ){
		for( i = old_cap; i < amount; i++ ){
			array->ctor( (char*)array->data+i*array->elem_size, array->allocator );
		}
	}
	array->capacity = old_cap+amount;
}
/*
 * calculate the size based on index.
 * currently fairly dumb, calculate what would be double by the time it gets there
 */ 
size_t ds_array_calc_grow_size( size_t capacity, size_t idx ){
	size_t new_cap = capacity;
	while( new_cap < idx ){
		new_cap *= 2;
	}
	return new_cap;

}


void* ds_array_set( ds_array_t array, size_t idx, void *data ){
	if( !array ) { return NULL; }
	size_t offset = idx*array->elem_size;
	if( idx >= array->capacity ){
		
		size_t grow_by = ds_array_calc_grow_size( array->capacity, idx );
		array->err("growing by %ld elements\n", grow_by );
		ds_array_grow( array, grow_by );
		/*array->err( "index %d is greater than the capacity of the array (%d)", idx, array->capacity  );*/
	}
	if( array->move ){
		array->move( (char*)array->data+offset, data );
	} else {
		void *element = (char*)array->data+offset;
		array->copy( &element,data,array->allocator);		
	}
	if( idx >= array->size ){
		array->size = idx+1;
	}
	return ((char*)array->data + offset);
}
/*
 * 
 */
void* ds_array_get( ds_array_t array, size_t idx ){
	if( !array ) { return NULL; }
	size_t offset = idx*array->elem_size;
	if( idx >= array->capacity ){
		array->err( "index %d is greater than the capacity of the array (%d)", idx, array->capacity  );
	}
	if ( idx >= array->size ){
		array->size = idx+1;
	}
	return ((char*)array->data + offset);
}
/*int  ds_array_grow( ds_array_t array, size_t idx ); */
/*int  ds_array_shrink( ds_array_t array, size_t amount ); */
void* ds_array_foreach( ds_array_t array, ds_traverse_f func, void *user ){
	int i = 0;
	if( !array ) { return NULL; }
	void *ret;
	for( i = 0; i < array->size; i++ ){
		ret = func( (char*)array->data+i*array->elem_size, user );
		if ( ret ) { return ret; };
	}

	return NULL;
}

size_t ds_array_size( ds_array_t arr ){
	if( !arr) { return -1;}
	return arr->size;
}

size_t ds_array_capacity( ds_array_t arr){
	if( !arr) { return -1;}
	return arr->capacity;
}

