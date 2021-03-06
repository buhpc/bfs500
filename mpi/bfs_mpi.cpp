/*
 * bfs_mpi.cpp
 * g++ -o bfs_omp bfs_omp.cpp -lrt -fopenmp
 */

#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <queue>
#include <omp.h>
#include <mpi.h>

#define VERTICES 10000
#define EDGES 10000

#define GIG 1000000000
#define CPG 2.60            // Cycles per GHz -- Adjust to your computer
#define NUM_THREADS 8

#include "bfs_mpi.h"

using namespace std;

int main() {
	int i;
	int j;

	// Initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm comm = MPI_COMM_WORLD;

	int myRank;
	int numParts;
	MPI_Comm_rank(comm, &myRank);
	MPI_Comm_size(comm, &numParts);

	srand(time(NULL));

	// graph represents the matrix
	int **graph = new int*[VERTICES];
	for (i = 0; i < VERTICES; i++) {	
    	graph[i] = new int[VERTICES]; 
  	}

	int size[VERTICES] = {};
	
	// visited contains visited positions
	int *visited = new int[VERTICES];

	// Load the graph
	for (i = 0; i < VERTICES; i++) {
		size[i] = rand() % EDGES;

		// Load the graph with the vertices
		for (j = 0; j < size[i]; j++) {
			int vertex = rand() % VERTICES;
			graph[i][j] = vertex;
		}
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	// Breadth first search
	for (i = 0; i < VERTICES; i++) {
		if (!visited[i]) {
			bfs(graph, size, i, visited);
		}
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	elapsedTime = diff(time1, time2);

	cout << "OpenMP BFS" << endl;
	printf("CPE: %ld\n", (long int)((double)(CPG) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec)));

	return 0;
}

void bfs(int** graph, int *size, int vertex, int *visited) {
	omp_set_num_threads(NUM_THREADS);	

	visited[vertex] = 1;

	int i;
	// double-ended queue
	deque<int> q;
	q.push_back(vertex);

	while (!q.empty()) {
		vertex = q.front();
		q.pop_front();

		for (i = 0; i < size[vertex]; i++) {
			if (!visited[graph[vertex][i]]) {
				visited[graph[vertex][i]] = 1;
				q.push_back(graph[vertex][i]);
			}
		}
	}
}

struct timespec diff(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec-start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}