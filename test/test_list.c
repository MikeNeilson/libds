#include "tinytest.h"
#include "list.h"


TINYTEST_DECLARE_SUITE(listsuite);

void copy_int( void **to, void *from, ds_allocator_t *alloc ){
	int **to_int = (int**)to;
    int *from_int = from;
    *to = alloc->malloc(sizeof(int));
	
	**to_int = *from_int;
}

void* simple_traverse_function(void *data, void *user){
    int *user_int = (int*)user;
    int *data_int = (int*)data;
    *user_int = *data_int;    
    return NULL;
}

int test_list_insert_back( const char *name ){
    printf("insert back test");
	ds_list_t list = NULL;
	list = ds_list_create(sizeof(int),DS_FUNC_COPY, copy_int, DS_END);
    TINYTEST_ASSERT_MSG( list != NULL, "list creation failed" );
    int result = 0;
	int first_value = 5;
	ds_list_insert_back(list, &first_value );
    ds_list_traverse(list, simple_traverse_function, &result);
    TINYTEST_ASSERT_MSG( first_value == result, "Value inserted isn't not in the list");

	int second_value = 6;
	ds_list_insert_back(list, &second_value );
    ds_list_traverse(list,simple_traverse_function, &result);
    TINYTEST_ASSERT_MSG( result == second_value, "2nd value inserted isn't at the end");


    ds_list_destroy(&list);
    TINYTEST_ASSERT_MSG( list == NULL, "list destruction failed");

	return 1;
}


TINYTEST_START_SUITE(listsuite);    
  TINYTEST_ADD_TEST(test_list_insert_back,NULL,NULL);
TINYTEST_END_SUITE();


TINYTEST_MAIN_SINGLE_SUITE(listsuite);