#include "list.h"
#include "common.h"
#include <string.h>
#include <stdarg.h>

struct ds_list_node_s{
	struct ds_list_node_s *next;
	void *data;
};

struct ds_list_s{
	ds_list_node_t head;
	ds_list_node_t tail;
	ds_copy_f copy;
	ds_move_f move;
	ds_ctor_f ctor;
	ds_dtor_f dtor;
	ds_allocator_t* alloc;
};

/*
 * create a new list, data size doesn't matter
 *
 *
 */

ds_list_t ds_list_create( size_t start_size, ... ){
	ds_ctor_f _ctor = NULL;
	ds_dtor_f _dtor = NULL;
	ds_copy_f _copy = NULL;
	ds_move_f _move = NULL;
	ds_error_f _err = ds_error_stderr;
	ds_allocator_t* allocator = NULL;
	ds_list_t list = NULL;
	int arg_type=DS_END;

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
				case DS_FUNC_MOVE:
					_move = va_arg(args, ds_move_f);
					break;
				case DS_FUNC_COPY:
					_copy = va_arg(args, ds_copy_f);
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

	list = (ds_list_t)allocator->calloc(1, sizeof( struct ds_list_s ));
	if ( !list ) {
		_err("unable to allocate memory for list structure" );
		return NULL;
	}
	list->alloc = allocator;
	list->dtor = _dtor;
	list->ctor = _ctor;
	list->move = _move;
	list->copy = _copy;

	return list;
}


void  ds_list_destroy( ds_list_t list ){
	ds_list_node_t cur=list->head,tmp=NULL;
	while( cur != NULL ){
		if( cur->next != NULL ){
			tmp = cur->next;
		}
		if( list->dtor != NULL ){
			list->dtor(cur->data, list->alloc);
			list->alloc->free(cur->data);
		}
		tmp = cur;
		cur = cur->next;
		list->alloc->free(tmp);
	}


}

/*
void  ds_list_free_elem( ds_list_t list, void *data ){

}*/
/*
 * TODO: may turn this into a generic ASSIGN -> COPY -> MOVE macro
 */
#define ASSIGN_OR_COPY( list,node, elem )\
	if( (list)->copy != NULL ) { \
		(list)->copy( &(node)->data, (elem), (list)->alloc ); \
	} \
	else { \
		(node)->data = (elem); \
	} 

ds_list_node_t ds_list_insert_front( ds_list_t list, void *data ){
	if( list->head == NULL ){
		list->head = list->alloc->malloc(sizeof(ds_list_node_t));
		list->tail = list->head;
		memset(list->head,0,sizeof(struct ds_list_node_s));
		ASSIGN_OR_COPY( list, list->head, data );
		//list->head->data = data; /* TODO: handle copy constructor */
	} else {
		ds_list_node_t tmp = list->head;
		//tmp->data = data;
		list->head = list->alloc->malloc(sizeof(ds_list_node_t));
		memset(list->head,0,sizeof(struct ds_list_node_s));
		ASSIGN_OR_COPY(list,list->head,data);
		list->head->next=tmp;


	}
	return list->head;
}

ds_list_node_t ds_list_insert_back( ds_list_t list, void *data ){
	if( list->head == NULL ){
		return ds_list_insert_front( list, data );
	} else {
		ds_list_node_t tmp = list->tail;
		list->tail->next = list->alloc->malloc(sizeof(ds_list_node_t));
		memset(list->tail->next,0,sizeof(struct ds_list_node_s));
		list->tail = list->tail->next;
		//list->tail->data = data;
		ASSIGN_OR_COPY(list,list->tail,data);
	}
	return list->tail;
}

void* ds_list_remove( ds_list_t list, void *data, ds_comparator_f func ){
	return NULL;
}

void* ds_list_traverse( ds_list_t list, ds_traverse_f func, void *user ){
	ds_list_node_t tmp = list->head;
	while( tmp != NULL ){
		func(tmp->data, user); 
		tmp = tmp->next;
	}
	return NULL;
}
