cmake_minimum_required(VERSION 3.0)
project(ds)

find_program(KCOV NAMES kcov)

set(CMAKE_C_STANDARD 99)



include_directories(src)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(
    NAME array_tests
    COMMAND test_array
)


add_test(
    NAME list_tests
    COMMAND test_list
)

add_custom_target(
    kcov_array
    COMMAND kcov ${CMAKE_BINARY_DIR}/coverage ${CMAKE_BINARY_DIR}/test/test_array
)

add_custom_target(
    kcov_list
    COMMAND kcov ${CMAKE_BINARY_DIR}/coverage ${CMAKE_BINARY_DIR}/test/test_list
)

add_custom_target(
     coverage
     #COMMAND cat ${CMAKE_BINARY_DIR}/coverage/*
     DEPENDS kcov_array kcov_list
)
