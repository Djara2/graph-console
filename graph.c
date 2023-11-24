#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "davestdlib.h"

typedef struct graph_node
{
	char *name;

	struct graph_node **neighbors;
	unsigned int neighbors_len;
	unsigned int neighbors_capacity;

	unsigned int *distances;
	unsigned int distances_len;
	unsigned int distances_capacity;

} graph_node;

graph_node* graph_node_create(char *name)
{
	graph_node *node = malloc(sizeof(graph_node));

	node->name = malloc(sizeof(char) * (strlen(name) + 1));
	node->name = strdup(name);

	// by default, there may be 3 neighbors. This gets expanded by the insert function as needed
	node->neighbors_len = 0;
	node->neighbors_capacity = 3;
	node->neighbors = malloc(sizeof(graph_node*) * node->neighbors_capacity);

	// (1) distances is a parallel array to the neighbors array. This makes it so that there does not need to
	// be a separate struct for edges. 
	// (2) Linear search is used to match the name to a node in the neighbors array, which is O(n), but since there 
	// are *probably* not going to be very many neighbors, this is not a problem. User enters a name to determine
	// which node they want to go to in application, because... well how else would they?
	node->distances_len = 0;
	node->distances_capacity = 3;
	node->distances = malloc(sizeof(unsigned int) * node->distances_capacity);

	return node;
}

bool graph_node_add_neighbor(graph_node *source, graph_node *destination, unsigned int distance)
{
	// ensure this node (identified by name) is not already in the neighbors -- if it is, then it cannot be added
	printf("DEBUG: entering neighbor add for loop\n");
	for(int i = 0; i < source->neighbors_len; i++)
	{
		printf("DEBUG: attempting name comparisons\n");
		if(strcmp( (source->neighbors[i])->name, (destination->name) )== 0)
		{
			printf("A node by the name \"%s\" already exists as a connection to node \"%s\".\n", (destination->name), (source->name));
			return false;
		}
	}

	// (1) allocate more memory if there is insufficient memory allocated to hold more neighbors
	if(source->neighbors_len >= source->neighbors_capacity)
	{
		printf("DEBUG: more memory is needed for source node neighbors buffer.\n");
		source->neighbors_capacity *= 2;
		printf("DEBUG: attempting realloc\n");
		source->neighbors = realloc(source->neighbors, sizeof(graph_node*) * (source->neighbors_capacity));
		if(source->neighbors == NULL)
		{
			perror("ERROR: failed to allocate more memory for neighbors buffer of graph node.");
			return false;
		}
	}
	printf("DEBUG: more memory created for neighbors buffer OR no reallocation necessary\n");
	// (2) insert destination node into the source's neighbors buffer
	source->neighbors[source->neighbors_len] = destination;
	(source->neighbors_len)++;

	// (3) insert distance value into source's distances buffer
	source->distances[source->distances_len] = distance;
	(source->distances_len)++;

	return true;
}

int main(int argc, char *argv[])
{
	// buffer to hold all created nodes
	unsigned int nodes_capacity = 5;
	unsigned int nodes_len = 0;
	graph_node **nodes = malloc(sizeof(graph_node*) * nodes_capacity);

	// variables for main program
	char user_selection_string[16];
	char *strtoul_ptr;
	unsigned int user_selection;
	char name[32];
	unsigned int source_index;
	unsigned int destination_index;
	unsigned int distance;
	bool continue_main_program = true;
	bool file_opened_successfully = false;

	// variables for parsing source file (start with a graph, saved in a file)
	unsigned int tokens_len = 0;
	unsigned int tokens_capacity = 5;
	char **tokens = malloc(sizeof(char*) * tokens_capacity);
	char *token;
	unsigned int line_len = 0;
	unsigned int line_capacity = 40;
	char *line = malloc(sizeof(char) * line_capacity);
	char c = '0';

	// create nodes and edges from input file, if one is provided
	if(argc >= 2)
	{
		FILE *source_file = fopen(argv[1], "r");

		// check if file actually exists
		if(source_file == NULL)
		{
			printf("Failed to read file \"%s\". No graph has been loaded.\n", argv[1]);
		}
		else file_opened_successfully = true;

		// if file actually exists, create graph
		if(file_opened_successfully)
		{
			// read the line
			while(true)
			{
				c = fgetc(source_file);
				// if a line is done, then a statement is done. 
				if(c == '\n' || c == EOF)
				{
					// terminate line
					insert_char(&line, &line_len, &line_capacity, '\0');

					// begin tokenizing the line
					token = strtok(line, ",");
					while(token != NULL)
					{
						// ensure there is enough space in the tokens buffer
						if(tokens_len >= tokens_capacity)
						{
							tokens_capacity *= 2;
							tokens = realloc(tokens, sizeof(char*) * tokens_capacity);
							if(tokens == NULL)
							{
								perror("Failed to allocate more memory for tokens buffer while parsing contents of graph input file.");
								return 1;
							}
						}
						// add token to tokens buffer
						tokens[tokens_len] = strdup(token);
						tokens_len++;

						// continue tokenizing
						token = strtok(NULL, ",");
					}

					// perform graph activities, based on the tokens provided
					if(strcmp(tokens[0], "node") == 0)        // case: new node
					{
						// ensure there is sufficient space to store a new node in the nodes buffer
						if(nodes_len >= nodes_capacity)
						{
							nodes_capacity *= 2;
							nodes = realloc(nodes, sizeof(graph_node*) * nodes_capacity);
							if(nodes == NULL)
							{
								perror("Failed to allocate more memory for all nodes buffer while creating new node from graph input file.");
								return 1;
							}
						}

						// add node to all nodes buffer
						nodes[nodes_len] = graph_node_create(tokens[1]);
						nodes_len++;

					}
					else if(strcmp(tokens[0], "edge") == 0)   // case: new edge between nodes
					{
						// nodes must already exist in the all nodes buffer
						// -> find indices of nodes in all nodes buffer
						for(int i = 0; i < nodes_len; i++)
						{
							if(strcmp(nodes[i]->name, tokens[1]) == 0) source_index = i;
							
							if(strcmp(nodes[i]->name, tokens[2]) == 0) destination_index = i;
						}

						// convert distance from str to long, create the connection
						distance = strtoul(tokens[3], &strtoul_ptr, 10);
						graph_node_add_neighbor(nodes[source_index], nodes[destination_index], distance);
					}
					else{}

					// reset tokens buffer
					for(int i = 0; i < tokens_len; i++)
					{
						free(tokens[i]);
					}
					tokens_len = 0;

					// reset the line
					line_len = 0;
					// jump out of loop once all statements have been read
					if(c == EOF) break;
				}
				// continue building the line
				else insert_char(&line, &line_len, &line_capacity, c);
			}
		}
		fclose(source_file);
	}



	// main program
	while(continue_main_program)
	{
		// display options menu
		printf("Please select an option:\n");
		printf("[1] Create a new node\n");
		switch(nodes_len)
		{
			case 0:
				break;
			default:
				printf("[2] Create a new connection between nodes\n");
				printf("[3] Print a node\n");
				printf("[4] Print all nodes\n");
				break;
		}
		printf("[5] Exit\n");

		// act based on the user's input
		printf("> ");
		fgets(user_selection_string, 16, stdin);
		user_selection_string[strlen(user_selection_string) - 1] = '\0';
		user_selection = strtoul(user_selection_string, &strtoul_ptr, 10); // typecast string into unsigned int 

		switch(user_selection)
		{
			// create a new node
			case 1:
				// determine the name of the node
				printf("What do you want to call this new node?: ");
				fgets(name, 32, stdin);
				name[strlen(name) - 1] = '\0'; 

				// create a new node and insert it into the graph nodes array
				// (1) ensure there is enough memory
				if(nodes_len >= nodes_capacity)
				{
					nodes_capacity *= 2;
					nodes = realloc(nodes, sizeof(graph_node*) * nodes_capacity);
					if(nodes == NULL)
					{
						perror("Unable to reallocate more memory for the nodes buffer.");
						return 1;
					}
				}
				nodes[nodes_len] = graph_node_create(name);
				nodes_len++;
				break;
			
			// create an edge between 2 nodes
			case 2:
				// display all nodes
				printf("The following nodes exist:\n");
				for(int i = 0; i < nodes_len; i++)
				{
					printf("[%d] %s\n", i, nodes[i]->name);
				}

				// select source and destination
				printf("Select a source node: ");
				fgets(user_selection_string, 16, stdin);
				user_selection_string[strlen(user_selection_string) - 1] = '\0';
				source_index = strtoul(user_selection_string, &strtoul_ptr, 10);
				printf("DEBUG: source_index = %u\n", source_index);

				printf("Select a destination node: ");
				fgets(user_selection_string, 16, stdin);
				user_selection_string[strlen(user_selection_string) - 1] = '\0';
				destination_index = strtoul(user_selection_string, &strtoul_ptr, 10);
				printf("DEBUG: destination_index = %u\n", destination_index);
				
				// get distance between these two points
				printf("Distance between %s and %s: ", nodes[source_index]->name, nodes[destination_index]->name);
				fgets(user_selection_string, 16, stdin);
				user_selection_string[strlen(user_selection_string) - 1] = '\0';
				distance = strtoul(user_selection_string, &strtoul_ptr, 10);
				printf("DEBUG: distance = %u\n", distance);

				printf("DEBUG: attempting to add neighbor");
				// create edge between the two
				graph_node_add_neighbor(nodes[source_index], nodes[destination_index], distance);
				break;

			// print a node and its connections
			case 3:
				// display all nodes
				printf("Select a node to print:\n");
				for(int i = 0; i < nodes_len; i++)
				{
					printf("[%d] %s\n", i, nodes[i]->name);
				}

				// select source and destination
				printf("> ");
				fgets(user_selection_string, 16, stdin);
				user_selection_string[strlen(user_selection_string) - 1] = '\0';
				source_index = strtoul(user_selection_string, &strtoul_ptr, 10);

				// introduce the node
				printf("Node \"%s\":\n", nodes[source_index]->name);

				printf("\tConnections:\n");
				// print all the connections it has
				for(int i = 0; i < nodes[source_index]->neighbors_len; i++)
				{
					printf("\t\t[%d] %s --- distance: %u\n", i, (nodes[source_index])->neighbors[i]->name, (nodes[source_index])->distances[i]);
				}
				break;

			// print all nodes and their connections
			case 4:
				// display all nodes
				for(int i = 0; i < nodes_len; i++)
				{
					// introduce the node
					printf("Node \"%s\":\n", nodes[i]->name);

					printf("\tConnections:\n");
					// print all the connections it has
					for(int j = 0; j < nodes[i]->neighbors_len; j++) 
					{
						printf("\t\t[%d] %s --- distance: %u\n", j, (nodes[i])->neighbors[j]->name, (nodes[i])->distances[j]);
					}
					printf("\n");
				}
				break;

			case 5:
				continue_main_program = false;
				break;

			default:
				printf("Menu option %u is invalid. No such menu option.\n", user_selection);
		}
		printf("\n");
		user_selection = 0;
	}

	// free the memory created for the nodes
	for(int i = 0; i < nodes_len; i++)
	{
		// clear the neighbors and distances buffers of the current node
		for(int j = 0; j < nodes[i]->neighbors_len; j++)
		{
			free(nodes[i]->neighbors[j]);
			free(nodes[i]->distances);
		}

		// actually free the current node
		free(nodes[i]);
	}
	return 0;
}