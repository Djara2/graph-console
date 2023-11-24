#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "davestdlib.h"

void insert_char(char **string_ptr, int *len, int *capacity, char c)
{
	if( (*len) >= (*capacity) )
	{
		(*capacity) *= 2;
		char *new_addr = realloc((*string_ptr), sizeof(char) * (*capacity));
		if(new_addr == NULL)
		{
			printf("Could not reallocate more memory to insert char \'%c\'\n", c);
			return;
		}
		(*string_ptr) = new_addr;
	}
	(*string_ptr)[(*len)] = c;
	(*len)++;
}

void concatenate(char **string_ptr, int *len, int *capacity, char *operand)
{
	int limit = strlen(operand);
	for(int i = 0; i < limit; i++)
	{
		if( (*len) >= (*capacity) )
		{
			(*capacity) *= 2;
			char *new_addr = realloc((*string_ptr), sizeof(char) * (*capacity));
			if(new_addr == NULL)
			{
				printf("Could not allocate more memory to concatenate strings.\n");
				return;

			}
			(*string_ptr) = new_addr;
		}
		(*string_ptr)[(*len)] = operand[i];
		(*len)++;
	}
}

// 1. modified to use strdup to stop destruction of source
char** split_string(char *source, char *delimiter)
{
	char *source_copy = strdup(source);
	int len = 0;
	int capacity = 10;
	char **string_tokens = malloc(sizeof(char*) * capacity);

	char *token = strtok(source_copy, delimiter);
	while(token != NULL)
	{
		string_tokens[len] = strdup(token);
		len = len + 1;
		if(len >= capacity)
		{
			capacity *= 2;
			string_tokens = realloc(string_tokens, sizeof(char*) * capacity);
		}
		token = strtok(NULL, delimiter);
	}
	// final element is NULL so that you can traverse
	string_tokens[len] = NULL;

	free(source_copy);
	return string_tokens;
}
