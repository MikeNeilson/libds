#include <stdlib.h>
#include <memory.h>
#include "tinytest.h"
#include "array.h"

TINYTEST_DECLARE_SUITE(arraysuite);

/***********
 *
 * Units tests
 *
 **********/

int test_array_create( const char *name ){
	/*printf("Running %s\n", name );*/
	ds_array_t arr = NULL;
	arr = ds_array_create( sizeof(int), 10, DS_END );
	TINYTEST_ASSERT_MSG(arr, "array creation failed" );
	ds_array_destroy( &arr );
	return 1;
}

int test_array_value_set_is_value_retrieved( const char *name ){
	/*printf("Running %s\n", name );*/
	ds_array_t arr = NULL;
	arr = ds_array_create( sizeof(int), 10, DS_END );
	int value_to_set = 4;
	ds_array_set( arr, 0, &value_to_set );
    int value_retrieved = *((int*)ds_array_get( arr, 0 ));
	TINYTEST_ASSERT_MSG( value_retrieved == value_to_set, "Unable to correctly set value" );
	ds_array_destroy( &arr );
	return 1;
}

int test_array_destroy(const char *name ){
	/*printf("Running %s\n", name );*/
	ds_array_t arr = ds_array_create(sizeof(int),10, DS_END);

	TINYTEST_ASSERT_MSG( arr, "array creation failed" );
	ds_array_destroy(&arr);
	TINYTEST_ASSERT_MSG( arr==NULL, "array destruction failed" );
	return 1;
}

typedef struct{
	int value;
	char *str;
} bigval;

void bigval_ctor( void *_obj, ds_allocator_t *allocator ){
	bigval *obj = (bigval*)_obj;
	obj->value=0;
	obj->str=NULL;
}

void bigval_dtor( void *_obj, ds_allocator_t *allocator ){
	bigval *obj = (bigval*)_obj;
	if( obj->str != NULL ){ 
		printf("dtor called on %s\n", obj->str);
		allocator->free( obj->str );
		obj->str = NULL;
	}
}

int test_array_complex_struct( const char *name ){
	/*printf("Running %s\n", name );*/
	bigval *tmp;
	bigval tmp2;
	memset(&tmp2, 0, sizeof(bigval) );
	ds_array_t array = ds_array_create( sizeof(bigval), 2,
					    DS_FUNC_CTOR, bigval_ctor,
					    DS_FUNC_DTOR, bigval_dtor,
					    DS_END );
	TINYTEST_ASSERT_MSG( array, "array creation failed" );
	
	tmp = ds_array_get(array, 0);
	tmp->value = 5;
	tmp->str = ds_default_allocator.calloc(1,6);
	memcpy( tmp->str, "hello", 5 );
	
	tmp = ds_array_get(array, 1);
	tmp->value = 6;
	tmp->str = ds_default_allocator.calloc(1,6);
	memcpy( tmp->str, "world", 5 );

	tmp = (bigval*)ds_array_get(array,0);
	TINYTEST_ASSERT_MSG( strcmp( tmp->str, "hello" ) == 0, "Array member not set" );


	tmp = calloc( 1,sizeof(bigval) );
	tmp->value = 7;
	tmp->str = calloc(1, 5);
	memcpy(tmp->str, "blah", 4 );

	ds_array_set( array, 2, tmp );
    size_t size = ds_array_size(array);
    size_t capacity = ds_array_capacity(array);
	printf("new size %ld", size );
	TINYTEST_ASSERT_MSG( capacity == 4, "array did not grow");

	TINYTEST_ASSERT_MSG( memcmp( ds_array_get(array,3), &tmp2, sizeof(bigval) ) == 0, "last val not properly cleared");
	bigval_dtor(tmp,&ds_default_allocator);
	ds_default_allocator.free(tmp);

	ds_array_destroy( &array );

	return 1;

}

int test_array_grow( const char *name ){
	ds_array_t array = ds_array_create( sizeof( int ), 10, DS_END );
	int v = 4;
	int v9 = 5;
	int *p = NULL;
	ds_array_set(array, 9, &v );

	ds_array_set( array, 10, &v );
    size_t capacity = ds_array_capacity(array);
	TINYTEST_ASSERT_MSG( capacity == 20, "array did not grow" );

	p = ds_array_get( array, 10 );
	TINYTEST_ASSERT_MSG( *p == v, "array value over capacity was not set correctly" );
	
	p = ds_array_get( array, 9 );
	TINYTEST_ASSERT_MSG( *p = v9, "array growth ate the late value");

	ds_array_destroy( &array );
	return 1;
}



TINYTEST_START_SUITE(arraysuite);
  TINYTEST_ADD_TEST(test_array_create,NULL,NULL);
  TINYTEST_ADD_TEST(test_array_destroy,NULL,NULL);
  TINYTEST_ADD_TEST(test_array_value_set_is_value_retrieved,NULL,NULL);  
  TINYTEST_ADD_TEST(test_array_complex_struct,NULL,NULL);
  TINYTEST_ADD_TEST(test_array_grow,NULL,NULL);
TINYTEST_END_SUITE();

TINYTEST_MAIN_SINGLE_SUITE(arraysuite);
