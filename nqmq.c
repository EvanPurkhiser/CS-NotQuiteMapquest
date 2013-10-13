#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define DATA_FILE_NAME    "nqmq.dat"
#define DATA_LINE_MAX_LEN 80

char **cities;
int  *distances;

int main(int argc, char *argv[])
{
	// Read in the needed information from the data file. This includes the city
	// names as well as the city edges (roads) between cities
	{
		char line[DATA_LINE_MAX_LEN];
		FILE *data_file;
		int  num_cities = 0;

		data_file = fopen(DATA_FILE_NAME, "r");

		// The first line will be the number of cities
		fscanf(data_file, "%d", &num_cities);
		fgets(line, DATA_LINE_MAX_LEN, data_file);

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
			distances[city_b][city_a] = dist;
		}
	}



	// Free up the cities array
	for (int i = 0; i < sizeof(cities) / sizeof(cities[0]); ++i)
		free(cities[i]);
	free(cities);

	return 0;
}
