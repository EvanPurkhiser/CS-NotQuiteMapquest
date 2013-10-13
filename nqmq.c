#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define DATA_FILE_NAME    "nqmq.dat"
#define DATA_LINE_MAX_LEN 80

char **cities;
int  *distances;

int main(int argc, char *argv[])
{
	// Step 1: Read file into cities array and distances adjacency matrix
	char line[DATA_LINE_MAX_LEN];
	FILE *data_file;
	int  num_cities = 0;

	data_file = fopen(DATA_FILE_NAME, "r");

	// The first line will be the number of cities
	fscanf(data_file, "%d", &num_cities);

	// Allocate space for the city names
	cities = malloc(sizeof(char*) * num_cities);

	// Read in all cities
	for (int i = 0; i < num_cities; ++i)
	{
		fgets(line, DATA_LINE_MAX_LEN, data_file);
		cities[i] = malloc(strlen(line) * sizeof(char));
		strcpy(cities[i], line);
	}

	// Allocate space for the city distances
	int (*distances)[num_cities] = malloc(sizeof(*distances) * num_cities),
		city_a = 0,
		city_b = 0,
		dist   = 0;

	// Now read in the adjacency list
	while (city_a != -1)
	{
		fgets(line, DATA_LINE_MAX_LEN, data_file);
		sscanf(line, "%d %d %d", &city_a, &city_b, &dist);
		distances[city_a][city_b] = dist;
	}


	// Clean things up
	for (int i = 0; i < num_cities; ++i)
		free(cities[i]);
	free(cities);

	return 0;
}
