#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <queue>
#include <omp.h>

#define VERTICES 500
#define EDGES 100 
#define STARTV 0 

#define GIG 10e9
#define CPG 2.90            
#define NUM_THREADS 4 

#include "bfs_omp.h"

using namespace std;

void printGraph(int **graph) {
	int i;

	for (i = 0; i < VERTICES; i++) {
    	cout << i << " ";
    	for (int j = 0; j < VERTICES; j++) {
    		if (graph[i][j] != 0) {
    			printf("%d ", graph[i][j]);
    		}
    	}
    	cout << endl;
    }
}

int main() {
	int i;

	// graph represents the matrix
	int **graph = new int*[VERTICES];
	for (i = 0; i < VERTICES; i++) {	
    	graph[i] = new int[VERTICES]; 
  	}

	int size[VERTICES] = {};

    int *visited = new int[VERTICES];

    // Load the graph
    old_populate_random(graph, size, VERTICES, EDGES);
    //populate_known(graph, size, VERTICES, EDGES);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (i = 0; < i < VERTICES; i++) {
        if (!visited[i]) {
            bfs(graph, size, visited, STARTV);
        }
    }
	
    clock_gettime(CLOCK_MONOTONIC, &time2);
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// elapsedTime = diff(time1, time2);

	cout << "OpenMP BFS" << endl;
	/*
	printf("CPE: %ld\n", (long int)((double)(CPG) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec)));
	long ms = (elapsedTime.tv_sec * 1000) + (elapsedTime.tv_nsec / 1.0e6);		
 	printf("Time: %ld (msec)\n", ms);
 	*/
 	double testTime = timeInSeconds(&time2)-timeInSeconds(&time1);
    printf("Millieconds: %lf\n", testTime * 1000.0);

    return 0;
}


void bfs(int** graph, int *size, int *visited, int vertex) {
    omp_set_num_threads(NUM_THREADS);
    int j, ilimit, next_vertex;
    deque<int> q;

    visited[vertex] += 1;
    q.push_back(vertex);

#pragma omp parallel default(shared) private(j, next_vertex) firstprivate(vertex, ilimit)
{
    while (!q.empty()) {
        #pragma omp critical (firstlock)
        {
        vertex = q.front();
        q.pop_front(); 
        ilimit = size[vertex];
        }

        #pragma omp for    
        for (j = 0; j < ilimit; j++) {
            next_vertex = graph[vertex][j];
            if (!visited[next_vertex]) {
	           visited[next_vertex] += 1;
                #pragma omp critical (scndlock)
                q.push_back(next_vertex);
            }
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

double timeInSeconds(struct timespec *t) {
    // a timespec has integer values for seconds and nano seconds
    return (t->tv_sec + 1.0e-9 * (t->tv_nsec));
}

void populate_random(int **graph, int *size, const int vertices, const int edges) {
    int i, j, sizev;
    queue<int> unassigned;
    srand(time(NULL));

    for (i = 0; i < vertices; i++) {
        unassigned.push(i); //Every vertex is initially unassigned
    }

    for (i = 0; i < vertices; i++) {
        sizev = rand() % edges + 2; //Each vertex connected to at least one other
        size[i] = sizev;
        graph[i][0] = i;
        for (j = 1; j < sizev; j++) {
            if(!unassigned.empty() && unassigned.front() != i) {
                graph[i][j] = unassigned.front();
                unassigned.pop();
            } else {
                graph[i][j] = rand() % vertices;
            }
        }
    }
}

void old_populate_random(int **graph, int *size, const int vertices, const int edges) {
    int i, j;
    srand(time(NULL));

    for (i = 0; i < vertices; i++) {
        size[i] = rand() % edges;
        for (j = 0; j < size[i]; j++) {
            graph[i][j] = rand() % vertices;
        }
    }
}

void populate_known(int **graph, int* size, const int vertices, const int edges) {
    int i, j;

    for (i = 0; i < vertices; i++) {
        size[i] = vertices;
        graph[i][0] = i;
        for (j = 1; j < size[i]; j++) {
            graph[i][j] = (j + i) % vertices;
        }
    }
}