#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "limits.h"

#define DATA_FILE_NAME    "nqmq.dat"
#define DATA_LINE_MAX_LEN 80

char **cities;
int  **distances;

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

		// Calculate how much memory we need to allocate for the adjacency
		// matrix
		int mem_size = num_cities * sizeof(int*) + num_cities *
			num_cities * sizeof(int);

		// Allocate the memory for the adjacency matrix
		distances = malloc(mem_size);

		// Set the row indexes as pointers to the columns
		for (int i = 0; i < num_cities; ++i)
		{
			distances[i] = (int*)(distances + num_cities+1) + (i * num_cities);
		}

		// All cities should have a infinite distance between them, we can
		// represent this with the INT_MAX constant
		for (int i = 0; i < num_cities; ++i)
		{
			for (int j = 0; j < num_cities; ++j)
			{
				distances[i][j] = INT_MAX;
			}
		}

		// All cities have a 0 distance between their selves
		for (int i = 0; i < num_cities; distances[i][i] = 0, ++i);

		// Fill in the edges that we know from the data
		while (1)
		{
			int city_a = 0,
				city_b = 0,
				dist   = 0;

			fscanf(data_file, "%d %d %d", &city_a, &city_b, &dist);
			fgets(line, DATA_LINE_MAX_LEN, data_file);

			if (city_a == -1) break;

			// The cities are _NOT_ zero indexed in the data file, we need}
			distances[city_a - 1][city_b - 1] = dist;
			distances[city_b - 1][city_a - 1] = dist;
		}
	}




	// Free up the cities array and distances array
	for (int i = 0; i < sizeof(cities) / sizeof(cities[0]); free(cities[i++]));
	free(cities);
	free(distances);

	return 0;
}
