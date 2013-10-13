#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define DATA_FILE_NAME    "nqmq.dat"
#define DATA_LINE_MAX_LEN 80

char **cities;
char *distances;

int main(int argc, char *argv[])
{
	// Step 1: Read file into cities array and distances adjacency matrix
	char line[DATA_LINE_MAX_LEN];
	FILE *data_file;

	data_file = fopen(DATA_FILE_NAME, "r");

	// The first line will be the number of cities
	fgets(line, DATA_LINE_MAX_LEN, data_file);
	int num_cities = atoi(line);

	// Allocate space for the city names
	cities = malloc(sizeof(char*) * num_cities);

	// Read in all cities
	for (int i = 0; i < num_cities; ++i)
	{
		fgets(line, DATA_LINE_MAX_LEN, data_file);
		cities[i] = malloc(strlen(line) * sizeof(char));
		strcpy(cities[i], line);
	}







	return 0;
}
