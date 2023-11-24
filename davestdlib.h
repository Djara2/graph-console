#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef DAVESTDLIB_H
#define DAVESTDLIB_H

void insert_char(char **string_ptr, int *len, int *capacity, char c);

void concatenate(char **string_ptr, int *len, int *capacity, char *operand);

char** split_string(char *source, char *delimiter);

#endif
