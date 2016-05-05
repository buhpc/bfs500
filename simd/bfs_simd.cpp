#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <queue>
#include <veclib.h>

#define VERTICES 1000
#define EDGES 10 

#define GIG 1000000000
#define CPG 2.90            // Cycles per GHz -- Adjust to your computer

#include "bfs_simd.h"

using namespace std;


void populate_random(int **graph, int *size, const int vertices, const int edges) {
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
        size[i] = i % edges;
        for (j = 0; j < size[i]; j++) {
            graph[i][j] = j % vertices;
        }
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
	
	// visited contains whether a vertex has been visited
	int *visited = new int[VERTICES];
        for (i = 0; i < VERTICES; i++) {
            visited[i] = 0;
        }

	// Generate the graphs
        //populate_random(graph, size, VERTICES, EDGES);
        populate_known(graph, size, VERTICES, EDGES);	

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	// Breadth first search
        bfs(graph, size, visited, VERTICES); 
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	elapsedTime = diff(time1, time2);

	cout << "Serial BFS" << endl;
	printf("CPE: %ld\n", (long int)((double)(CPG) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec))); 
    
        //Validation - Checks each vertex was visited once
        for (i = 0; i < VERTICES; i++) {
            if (visited[i] != 1) {
                printf("visited[%d] was %d\n", i, visited[i]);
            }
        }

	return 0;
}

void bfs(int** graph, int *size, int *visited, int vertices) {
    // double-ended queue
    deque<int> q;
    int i, j, vertex, next_vertex; 

    for (i = 0; i < vertices; i++) {
	if (!visited[i]) {
            visited[i] += 1;
	    q.push_back(i);
            
            while (!q.empty()) {
		vertex = q.front();
		q.pop_front();

		for (j = 0; j < size[vertex]; j++) {
                    next_vertex = graph[vertex][j];
	            if (!visited[next_vertex]) {
			visited[next_vertex] += 1;
			q.push_back(next_vertex);
		    }
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
