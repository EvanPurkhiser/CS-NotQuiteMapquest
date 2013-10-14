# Not Quite Map Quest - Lab 3

Evan Purkhiser

## Problem description

The goal of this project was to use the Floyd-Warshall algorithm to determine the
shortest path on a graph with no cycles between two nodes. The basic idea of
this algorithm is to iterate over each node and look at it as if it was a
intermediate node between two other nodes. We then check if the path between two
nodes traveling through the intermediate is shorter than what was previously
recorded for those two nodes, updating the distance. We can also keep track of
what intermediate node must be traveled through so we can reconstruct the
shortest path.

It is, of course, possible to parallelize this by splitting up the sections of
the adjacency matrix that should be checked into separate threads. The trick is
to sync up each thread before moving onto the next intermediate node.  If the
threads were not synced, this would lead to a incomplete or incorrect adjacency
matrix.

## Parallelized performance

I constructed the program so the user may specify the number of threads to
execute with as the first parameter of the command:

    ./nqmq 3 # Execute with 3 threads

By default, the program will execute with only one thread of execution.

Each thread-count trial was conducted 10 times using the provided data file
containing 28 nodes and 74 edges. The average execution time of the threads was
then taken.

 * When running with **1 thread** of execution, it takes an average of
   $249.2\mu$ seconds to calculate the complete adjacency matrix for the city
   graph.

 * When running with **2 threads** of execution, it takes an average of
   $473.3\mu$ seconds to calculate the complete adjacency matrix for the city
   graph.

 * When running with **4 threads** of execution, it takes an average of
   $959.2\mu$ seconds to calculate the complete adjacency matrix for the city
   graph.

 * When running with **7 threads** of execution, it takes an average of
   $1370.8\mu$ seconds to calculate the complete adjacency matrix for the city
   graph.

This was conducted on a Machine running Linux 3.11 with a Intel i7 3770k
@3.5GHz. This particular processor has 4 cores, with two "hyperthreads" per
core.

## Why does it take longer with more threads?

At first, after completing the program and seeing the execution times of the
algorithm increase as it was executed with more threads, I was sure something
had to be wrong.

However, after some investigation, I believe our data set is actually much too
small for calculating the adjacency matrix to be worth parallelizing. The
overhead from creating the thread and iterating for each thread actually
outweighs the benefit of doing each adjacency matrix section calculation in
different threads.

I conducted a small experiment to determine if my theory was correct. The
following code snippet is an excerpt from the bulk of the threaded function of
my program:

    for (int k = 0; k < num_cities; ++k)
	{
		// Wait for all threads before we do shortest path calculations on the
		// adjacency matrix for this intermediate city
		pthread_barrier_wait(&calc_barrier);

		for (int i = start; i < end; ++i)
		{
			for (int j = 0; j < num_cities; ++j)
			{
				// Do the adjacency matrix shortest path calculations
			}
		}
	}

The problem here is that the overhead of setting this all up actually takes
longer than doing the adjacency matrix shortest path calculation. So to simulate
a larger data-set I've inserted a `usleep` just before doing the calculations:

	usleep((useconds_t) 100);
	// Do the adjacency matrix shortest path calculations

Now when executing the program I see the following times:

 * With just 1 thread it takes about 3.5 seconds to calculate the shortest paths
   adjacency matrix.

 * With 7 threads of execution it takes about a half a seconds to calculate the
   shortest paths adjacency matrix.

From this, we can conclude that it would indeed be worth parallelizing the
calculations if we had many, many, more nodes to calculate. The calculation time
would then overpower the overhead time and we would see a performance increase.
