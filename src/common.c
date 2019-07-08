#include "common.h"
#include <stdarg.h>

ds_allocator_t ds_default_allocator = {
	malloc,
	realloc,
	calloc,
	free
};


void ds_error_stderr( const char *fmt, ...){
	va_list args;
	va_start( args, fmt );
	vfprintf( stderr, fmt, args );
	va_end( args );
}

void ds_error_null( const char *fmt, ... ){
	return;
}

