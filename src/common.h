#ifndef DS_COMMON_H
#define DS_COMMON_H
#include <stdlib.h>
#include <stdio.h>


/* function pointers */
typedef int (*ds_comparator_f)( void *a, void *b);
typedef void (*ds_ctor_f)( void *self );
typedef void (*ds_copy_f)( void *to, void *from );
typedef void (*ds_move_f)( void *to, void *from );
typedef void (*ds_dtor_f)( void *self );
typedef void* (*ds_traverse_f)( void *obj, void *user );

typedef void (*ds_error_f)( const char *fmt, ... );

/* default functions */
void ds_error_stderr( const char *fmt, ...);
void ds_error_null( const char *fmt, ... );

/* memory handling */
typedef void* (*ds_malloc_t)( size_t size );
typedef void* (*ds_realloc_t)( void *ptr, size_t newsize );
typedef void* (*ds_calloc_t)( size_t num_elem, size_t elem_size);
typedef void  (*ds_free_t)( void *ptr );

typedef struct {
	ds_malloc_t malloc;
	ds_realloc_t realloc;
	ds_calloc_t calloc;
	ds_free_t free;
} ds_allocator_t;

/* default allocator */
extern ds_allocator_t ds_simple_allocator;

enum ds_constants {
	DS_FUNC_CTOR,
	DS_FUNC_DTOR,
	DS_FUNC_COPY,
	DS_FUNC_MOVE,
	DS_FUNC_ERR,
	DS_TYPE_ALLOCATOR,


	DS_END
};










#endif
