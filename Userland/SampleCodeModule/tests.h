#ifndef TESTS_H
#define TESTS_H

#include "test_util.h"
#include "libc/include/stdio.h"

void test_processes_wrapper(int argv, char* argc[]);
uint64_t test_mm(uint64_t argc, char *argv[]);

#endif // TESTS_H