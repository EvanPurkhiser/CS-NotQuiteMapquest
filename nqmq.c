#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define DATA_FILE_NAME "nqmq.dat"
#define DATA_LINE_MAX_LEN 80

char **cities;
int  **distances;
int  **through;

int num_cities = 0;

// By default use one thread of execution
int num_threads = 1;

// We need a barrier to sync up the threads before doing the next iteration of
// calculations
pthread_barrier_t calc_barrier;

void *calculate_shorest_paths(void *arg)
{
	int id = *(int*) arg;

	printf("Syncing thread id %d first\n", id);

	pthread_barrier_wait(&calc_barrier);

	printf("Syncing thread id %d second\n", id);

	return NULL;
}

int main(int argc, char *argv[])
{
	puts("\e[0;34m==>\e[0m Reading in values from the data file...");

	// Read in the needed information from the data file. Using this data we can
	// setup the cities array and distances matrix. We will also setup the
	{
		char line[DATA_LINE_MAX_LEN];
		FILE *data_file;

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

		printf("    Read in %d cities...\n", num_cities);

		// Calculate how much memory we need to allocate for the adjacency
		// matrix
		int mem_size = num_cities * sizeof(int*) + num_cities *
			num_cities * sizeof(int);

		// Allocate the memory for the adjacency matrix and through matrix
		distances = malloc(mem_size);
		through   = malloc(mem_size);

		// Set the row indexes as pointers to the columns
		for (int i = 0; i < num_cities; ++i)
		{
			distances[i]  = (int*)(distances + num_cities+1) + (i * num_cities);
			through[i]    = (int*)(through   + num_cities+1) + (i * num_cities);
		}

		// All cities should have a infinite distance between them, we can
		// represent this with the INT_MAX constant
		for (int i = 0; i < num_cities; ++i)
		{
			for (int j = 0; j < num_cities; ++j)
			{
				distances[i][j] = INT_MAX;
				through[i][j]   = -1;
			}
		}

		// All cities have a 0 distance between their selves
		for (int i = 0; i < num_cities; distances[i][i] = 0, ++i);

		// Fill in the edges that we know from the data
		int total;
		for (total = 0; 1; ++total)
		{
			int city_a = 0,
				city_b = 0,
				dist   = 0;

			fscanf(data_file, "%d %d %d", &city_a, &city_b, &dist);
			fgets(line, DATA_LINE_MAX_LEN, data_file);

			if (city_a == -1) break;

			// The cities are _NOT_ zero indexed in the data file
			distances[city_a - 1][city_b - 1] = dist;
			distances[city_b - 1][city_a - 1] = dist;
		}

		printf("    Read in %d connecting roads with distances...\n", total);
		fclose(data_file);
	}

	// The number of threads to use is the first argument
	if (argc > 1)
	{
		num_threads = atoi(argv[1]);
	}

	if (num_cities % num_threads != 0)
	{
		printf("\e[0;31m==> %d threads is not evenly divisible into %d cities\n",
			num_threads, num_cities);
		exit(-1);
	}

	printf("\e[0;34m==>\e[0m Starting up %d threads to calculate shortest paths...\n", num_threads);

	pthread_t threads[num_threads];
	int thread_ids[num_threads];

	pthread_barrier_init(&calc_barrier, NULL, num_threads);

	for (int i = 0; i < num_threads; ++i)
	{
		thread_ids[i] = i;
		pthread_create(&threads[i], NULL, calculate_shorest_paths, &thread_ids[i]);
	}

	// Wait for calculations to finish
	for (int i =0; i < num_threads; pthread_join(threads[i++], NULL));

	puts("\e[0;34m==>\e[0m Finished calculating shortest paths.");


	// Free up the cities array and distances array
	for (int i = 0; i < sizeof(cities) / sizeof(cities[0]); free(cities[i++]));
	free(cities);
	free(distances);

	return 0;
}
