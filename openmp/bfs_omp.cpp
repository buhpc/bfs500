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
#define NUM_THREADS 8 

#include "bfs_omp.h"

using namespace std;


int main() {
    int i;

    // graph represents the matrix
    int **graph = new int*[VERTICES];
    for (i = 0; i < VERTICES; i++) {	
    	graph[i] = new int[VERTICES+EDGES+2]; 
    }

    int size[VERTICES] = {};
	
    // visited contains visited positions
    int *visited = new int[VERTICES];
    for (i = 0; i < VERTICES; i++) {
        visited[i] = 0;
    }
    // Load the graph
    populate_random(graph, size, VERTICES, EDGES);
    //populate_known(graph, size, VERTICES, EDGES);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    bfs(graph, size, visited, STARTV);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    elapsedTime = diff(time1, time2);
	
    printf("%ld\n", (long int)((double)(10e-3) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec)));

    //Validation - checks that every vertex in tree was visted once
    for (i = 0; i < VERTICES; i++) {
        if (visited[i] != 1) {
            printf("Visited[%d] was %d\n", i, visited[i]);
        } 
    }

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

